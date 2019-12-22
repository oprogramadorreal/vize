#version 330 core

// x and y must be in [-1, 1]
layout(location = 0) in vec2 vertexPosition_ndc;

out vec2 UV;

void main() {
    gl_Position = vec4(vertexPosition_ndc, 0, 1);
	UV = (vertexPosition_ndc + vec2(1, 1)) / 2.0;
}