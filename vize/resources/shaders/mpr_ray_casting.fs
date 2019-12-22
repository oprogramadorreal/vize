#version 330 core

in vec2 UV;
out vec3 fragmentColor;

uniform vec3 cameraPosition_normmodelspace;
uniform vec3 lightPosition_normmodelspace;

uniform sampler3D volumeSampler;
uniform sampler1D transferFunction;
uniform sampler2D raysOriginTexture;
uniform sampler2D raysEndTexture;

uniform float xPlanePosition = -1.0; // -1.0 means "disabled"
uniform float yPlanePosition = -1.0;
uniform float zPlanePosition = -1.0;

uniform bool lightingEnabled = false;
uniform bool cuttingPlanes = false;

uniform vec3 volumeSpacing; // dataset voxel size
uniform float sampleStep = 0.0025;

uniform vec3 backgroundColor = vec3(0.2, 0.2, 0.2);

// light properties // TODO: use uniforms
const vec3 lightColor = vec3(1, 1, 1);
const float lightPower = 2.5f;

// Sample volume (3D texture).
float sample(const vec3 position_01) {
    return texture(volumeSampler, position_01).r;
}

// Sample transfer function (2D texture).
vec4 transfer(const float sampleValue) {
    return texture(transferFunction, sampleValue);
}

// Calculate the volume gradient at @param samplePosition_01.
vec3 gradient(const vec3 samplePosition, const float sampleValue) {
	float v = sampleValue;

	float v0 = sample(samplePosition + vec3(volumeSpacing.x, 0.0, 0.0));
	float v1 = sample(samplePosition + vec3(0.0, volumeSpacing.y, 0.0));
	float v2 = sample(samplePosition + vec3(0.0, 0.0, volumeSpacing.z));

	return vec3(v - v0, v - v1, v - v2);
}

// Calculate the volume gradient at @param samplePosition_01 based on alpha channel.
// Based on Hadwiger et al. "Techniques for GPU-Based Volume Raycasting Techniques for GPU Volume Raycasting", 2008, page 62.
vec3 gradientOnAlpha(const vec3 samplePosition, const float sampleAlpha) {
	float v = sampleAlpha;

	float v0 = transfer(sample(samplePosition + vec3(volumeSpacing.x, 0.0, 0.0))).a;
	float v1 = transfer(sample(samplePosition + vec3(0.0, volumeSpacing.y, 0.0))).a;
	float v2 = transfer(sample(samplePosition + vec3(0.0, 0.0, volumeSpacing.z))).a;

	return vec3(v - v0, v - v1, v - v2);
}

// Apply Phong shading to a sample color.
vec4 shade(const vec4 sampleColor, const vec3 samplePosition, const vec3 sampleNormal) {
	vec3 materialDiffuseColor = sampleColor.rgb;
	vec3 materialAmbientColor = vec3(0.1, 0.1, 0.1) * materialDiffuseColor;
	vec3 materialSpecularColor = vec3(0.6, 0.6, 0.6);

	// distance from sample to light
	float lightDistance = length(lightPosition_normmodelspace - samplePosition);
	float lightDistance2 = lightDistance * lightDistance;

    // sample normal vector
    vec3 N = sampleNormal;

    // light vector: goes from sample to light
    vec3 L = normalize(lightPosition_normmodelspace - samplePosition);

	// cosine of the angle between the normal and the light direction, clamped above 0
	float cosTheta = clamp(dot(N, L), 0, 1);

	// eye vector: goes from sample to camera
	vec3 E = normalize(cameraPosition_normmodelspace - samplePosition);

    // reflect vector: reflects the direction from light to sample by N (sample normal) // R = (-L) - 2.0 * dot(N, (-L)) * N.
    vec3 R = reflect(-L, N);

	// cosine of the angle between the eye vector (E) and the reflect vector (R), clamped to 0
	float cosAlpha = clamp(dot(E, R), 0, 1);

    vec3 shadedSampleColor = materialAmbientColor;
	shadedSampleColor += materialDiffuseColor * lightColor * lightPower * cosTheta / lightDistance2;
	shadedSampleColor += materialSpecularColor * lightColor * lightPower * pow(cosAlpha, 5) / lightDistance2;

    return vec4(shadedSampleColor, sampleColor.a);
}

struct Plane {
    vec3 normal;
    float d;
};

Plane planesMPR[3];

float distance(const Plane plane, const vec3 point) {
    return dot(plane.normal, point) - plane.d;
}

bool testPlaneSegment(const Plane plane, const vec3 a, const vec3 b, out vec3 intersection) {
    vec3 ab = b - a;
    float t = -distance(plane, a) / dot(plane.normal, ab);

	if (t < 0.0001 || t > 1.0001) {
        return false;
	}

    intersection = a + t*ab;

    return true;
}

float fuzzyBelongs(float value, float center, float centerLeft, float centerRight) {
	if (value < centerLeft || value > centerRight) {
		return 0.0;
	} else {
		if (value < center) {
			return (value - centerLeft) / (center - centerLeft);
		} else { // value > center
			return (centerRight - value) / (centerRight - center);
		}
	}
}

vec4 compose(const vec4 colorAcc, const vec4 sampleColor) {
//    vec3 newRGB = colorAcc.rgb + (1.0 - colorAcc.a) * sampleColor.rgb;
//    float newA = colorAcc.a + (1.0 - colorAcc.a) * sampleColor.a;
//    return vec4(newRGB, newA);

    float composedAlpha = (1.0 - colorAcc.a) * sampleColor.a;
    return colorAcc + vec4(composedAlpha * sampleColor.rgb, composedAlpha);
}

#define MAX_DISTANCE 1000000.0

// TODO: use uniforms?
#define ALPHA_THRESHOLD 0.99
#define MIN_ALPHA 0.05

// Perform ray casting for the current fragment.
vec3 raycast(const vec3 rayOrigin, const vec3 rayDirection, const float rayLength, float rayInitialDistance) {
    vec4 colorAcc = vec4(0, 0, 0, 0);

    vec3 rayEnd = rayOrigin + rayLength * rayDirection;

    bool sameSide = (planesMPR[0].d != -1.0) || (planesMPR[1].d != -1.0) || (planesMPR[2].d != -1.0); // some MPR plane is active
        
    // calculate the distance from rayOrigin to each MPR plane
    for (int i = 0; i < 3; ++i) {
        if (planesMPR[i].d != -1.0) { // if plane is enabled
            float rayDistanceToMPR = distance(planesMPR[i], rayOrigin);
            float cameraDistanceToMPR = distance(planesMPR[i], cameraPosition_normmodelspace);

            float rayStartsOnPlaneEdge = fuzzyBelongs(rayDistanceToMPR, 0.0, -0.0025, 0.0025);
            
            if (rayStartsOnPlaneEdge > 0.0) { // draw line on plane border
                colorAcc += vec4(1, 1, 1, 1) * rayStartsOnPlaneEdge;
                sameSide = false;
            } else {
                sameSide = sameSide && ( (rayDistanceToMPR > 0.0) == (cameraDistanceToMPR > 0.0) );
            }
        }
    }
        
    vec3 mprClosestIntersection;
    float mprClosestDistance = MAX_DISTANCE;

    // calculate the closest intersection of the ray with MPR planes
    for (int i = 0; i < 3; ++i) {
        if (planesMPR[i].d != -1.0) { // if plane is enabled
            vec3 intersection;

            if (testPlaneSegment(planesMPR[i], rayOrigin, rayEnd, intersection)) {
                float distance = length(intersection - rayOrigin);

                if (distance < mprClosestDistance) {
                    mprClosestDistance = distance;
                    mprClosestIntersection = intersection;
                }
            }
        }
    }

    if (sameSide) {
        if (mprClosestDistance == MAX_DISTANCE) {
            return backgroundColor; // did not intersect with any MPR plane... draw background
        }
        
        if (cuttingPlanes) {
            rayInitialDistance += mprClosestDistance; // make ray begins on MPR intersection
        } else {
            float sampleValue = sample(mprClosestIntersection);
            vec4 sampleColor = transfer(sampleValue);
            return sampleColor.rgb; // sample the color on MPR intersection and return
        }
    } else {
        if (!cuttingPlanes && (mprClosestDistance != MAX_DISTANCE)) {
            float sampleValue = sample(mprClosestIntersection);
            vec4 sampleColor = transfer(sampleValue);
            colorAcc.rgb += sampleColor.rgb;
        }
    }

    // volume ray casting starts here
    float t = rayInitialDistance; // sample ray in [t, rayLength] range

    do {
        vec3 rayPosition = rayOrigin + rayDirection * t;

        // sample the volume at current position
        float sampleValue = sample(rayPosition);

        // calculate a color for this sample value
        vec4 sampleColor = transfer(sampleValue);

        if (sampleColor.a >= MIN_ALPHA) { // if this color has a significant alpha value
            if (lightingEnabled) {
                // calculate a normal vector at the current sample position
                //vec3 sampleNormal = normalize(gradientOnAlpha(rayPosition, sampleColor.a));
                vec3 sampleNormal = normalize(gradient(rayPosition, sampleValue));

                // apply shading
                sampleColor = shade(sampleColor, rayPosition, sampleNormal);
            }

             // compose sample color with accumulated color
            colorAcc = compose(colorAcc, sampleColor);
        }
       
        // move to the next position
        t += sampleStep;

    } while ((colorAcc.a < ALPHA_THRESHOLD) && (t <= rayLength));

    return colorAcc.rgb;
}

void main() {
    // setup MPR planes
    planesMPR[0].normal = vec3(1,0,0);
    planesMPR[0].d = xPlanePosition;
    planesMPR[1].normal = vec3(0,1,0);
    planesMPR[1].d = yPlanePosition;
    planesMPR[2].normal = vec3(0,0,1);
    planesMPR[2].d = zPlanePosition;

    // setup fragment ray
    vec3 rayOrigin = texture(raysOriginTexture, UV).rgb;
    vec3 rayEnd = texture(raysEndTexture, UV).rgb;
    vec3 rayDirection = rayEnd - rayOrigin;
    float rayLength = length(rayDirection);

    if (rayLength < 0.01) {
        fragmentColor = backgroundColor; // ray is too short... draw background
    } else {
        // normalize ray direction
        rayDirection /= rayLength;

        // use interleaved sampling, as described by Scharsach, H. "Advanced GPU Raycasting".
        float rayInitialDistance = sampleStep * fract(length(UV * vec2(1074, 750)));

        fragmentColor = raycast(rayOrigin, rayDirection, rayLength, rayInitialDistance);
    }
}