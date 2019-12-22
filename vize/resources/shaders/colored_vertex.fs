#version 330 core

in vec3 fragmentColor;
layout(location = 0) out vec3 color;

void main() {
	color = fragmentColor;
}