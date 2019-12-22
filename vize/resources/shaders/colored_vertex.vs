#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 M = mat4(1); // model matrix - from model to world space
uniform mat4 V = mat4(1); // view matrix - from world to camera space
uniform mat4 P = mat4(1); // projection matrix - from camera to NDC space

void main() {
	mat4 MVP =  P * V * M;
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColor = vertexColor;
}