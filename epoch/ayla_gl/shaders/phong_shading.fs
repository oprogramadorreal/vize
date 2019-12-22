#version 330 core

// input: interpolated values from the vertex shader
in vec2 UV;
in vec3 vertexPosition_worldspace; // fragment position (interpolated from vertex position)
in vec3 vertexNormal_cameraspace;
in vec3 eyeDirection_cameraspace; // direction from fragment to camera
in vec3 lightDirection_cameraspace; // direction from fragment to light

// output: fragment color
out vec4 color;

uniform vec4 materialDiffuseColor = vec4(1);
uniform vec4 materialSpecularColor = vec4(0, 0, 0, 1);
uniform vec4 materialAmbientColor = vec4(0.1, 0.1, 0.1, 1);
uniform vec3 lightPosition_worldspace;
uniform sampler2D textureSampler;
uniform bool textureEnabled = false;

// light emission properties
const vec4 lightColor = vec4(1, 1, 1, 1);
uniform float lightPower = 30.0f;
uniform bool lightQuadraticAttenuation = true;

void main() {
	// material properties
    
    vec4 diffuseColor = materialDiffuseColor;

    if (textureEnabled) {
        diffuseColor *= texture(textureSampler, UV);
    }

	vec4 ambientColor = materialAmbientColor * diffuseColor;
	vec4 specularColor = materialSpecularColor;

	// distance from fragment to light
	float lightDistance = length(lightPosition_worldspace - vertexPosition_worldspace);
	float usedLightDistance = lightQuadraticAttenuation ? (lightDistance * lightDistance) : 1.0;

	vec3 N = normalize(vertexNormal_cameraspace); // fragment normal (interpolated from vertex normal)
	vec3 L = normalize(lightDirection_cameraspace); // direction from the fragment to the light

    if (!gl_FrontFacing) {
        N = -N;
    }

	// cosine of the angle between the normal and the light direction
	float cosTheta = max(dot(N, L), 0.0);

	vec3 E = normalize(eyeDirection_cameraspace); // direction from fragment to camera
	vec3 R = reflect(-L, N); // reflects direction from light to fragment for fragment normal // R = (-L) - 2.0 * dot(N, (-L)) * N.

	// cosine of the angle between the eye vector E and the reflect vector R, clamped to 0
	float cosAlpha = clamp(dot(E, R), 0, 1);

    // calculate the output color
    vec4 fragColor = ambientColor;
	fragColor += diffuseColor * lightColor * lightPower * cosTheta / usedLightDistance;
	fragColor += specularColor * lightColor * lightPower * pow(cosAlpha, 5) / usedLightDistance;

	fragColor.a = diffuseColor.a;
	
	color = fragColor;
}