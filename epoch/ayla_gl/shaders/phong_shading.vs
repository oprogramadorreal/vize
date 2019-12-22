#version 330 core

// input vertex data
// @see TriangleMeshGLObjects
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

// output data
out vec2 UV;
out vec3 vertexPosition_worldspace;
out vec3 vertexNormal_cameraspace;
out vec3 eyeDirection_cameraspace; // direction from vertex to camera
out vec3 lightDirection_cameraspace; // direction from vertex to light

// shader parameters
uniform mat4 M = mat4(1); // model matrix - from model to world space
uniform mat4 V = mat4(1); // view matrix - from world to camera space
uniform mat4 P = mat4(1); // projection matrix - from camera to NDC space
uniform vec3 lightPosition_worldspace = vec3(0);

void main() {
	mat4 MV = V * M;
	mat4 MVP = P * MV;

	// output position of the vertex, in NDC space
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);

	// position of the vertex in worldspace
	vertexPosition_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;

	// vector that goes from the vertex to the camera, in camera space.
	vec3 vertexPosition_cameraspace = (V * vec4(vertexPosition_worldspace, 1)).xyz;
	eyeDirection_cameraspace = vec3(0, 0, 0) - vertexPosition_cameraspace; // in camera space, camera is at (0, 0, 0)

	// vector that goes from the vertex to the light, in camera space
	vec3 lightPosition_cameraspace = (V * vec4(lightPosition_worldspace, 1)).xyz;
	lightDirection_cameraspace = lightPosition_cameraspace - vertexPosition_cameraspace;

	// normal of the the vertex, in camera space
	vertexNormal_cameraspace = (MV * vec4(vertexNormal_modelspace, 0)).xyz;

	UV = vertexUV;
}

