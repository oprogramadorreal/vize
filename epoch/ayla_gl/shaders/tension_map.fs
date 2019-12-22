#version 330 core

// input: interpolated values from the vertex shader
in float tension;

// output: fragment color
out vec4 color;

uniform float minTension = 0.5;
uniform float maxTension = 1.5;

/**
 * Return a RGB colour value given a scalar v in the range [vMin,vMax]. In this case each colour
 * component ranges from 0 (no contribution) to 1 (fully saturated), modifications for other ranges
 * is trivial. The colour is clipped at the end of the scales if v is outside the range [vMin,vMax].
 *
 * @author Paul Bourke - http://paulbourke.net/miscellaneous/colourspace/
 * @see http://stackoverflow.com/questions/7706339/grayscale-to-red-green-blue-matlab-jet-color-scale
 */
vec3 getColor(float v, float vMin, float vMax) {
    v = clamp(v, vMin, vMax);
    float dv = vMax - vMin;

    vec3 c = vec3(1.0, 1.0, 1.0);

    if (v < (vMin + 0.25 * dv)) {
        c.r = 0.0;
        c.g = 4.0 * (v - vMin) / dv;

    } else if (v < (vMin + 0.5 * dv)) {
        c.r = 0.0;
        c.b = 1.0 + 4.0 * (vMin + 0.25 * dv - v) / dv;

    } else if (v < (vMin + 0.75 * dv)) {
        c.r = 4.0 * (v - vMin - 0.5 * dv) / dv;
        c.b = 0.0;

    } else {
        c.g = 1.0 + 4.0 * (vMin + 0.75 * dv - v) / dv;
        c.b = 0.0;
    }

    return c;
}

void main() {
	color = vec4(getColor(tension, minTension, maxTension), 1.0);
}