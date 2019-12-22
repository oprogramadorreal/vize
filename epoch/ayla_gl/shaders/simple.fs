#version 330 core

uniform vec4 materialColor = vec4(0, 0, 0, 1);

out vec4 color;

void main() {
	color = materialColor;
}