/**
 * Example showing how to use ayla::FPSCamera abstraction.
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ayla_gl/fps_camera.hpp>
#include <ayla_gl/shaders/phong_shading_program.hpp>
#include <ayla_gl/renderers/triangle_mesh_renderer.hpp>

#include <ayla/geometry/triangle_mesh_builder.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

auto _windowWidth = unsigned(1024u);
auto _windowHeight = unsigned(768u);

glm::mat4 _view;
glm::mat4 _projection;

ayla_gl::FPSCamera _camera(glm::vec3(0, 0, 5));
const auto _cameraRotationSpeed = ayla::Float(0.005f);
const auto _cameraTranslationSpeed = ayla::Float(3.0f);
double _mouseX, _mouseY;

void mouseMoved(GLFWwindow* window, double xpos, double ypos) {
	const auto xDiff = ayla::Float(xpos - _mouseX);
	const auto yDiff = ayla::Float(ypos - _mouseY);
	_mouseX = xpos;
	_mouseY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		_camera.pitch(yDiff * -_cameraRotationSpeed);
		_camera.yaw(xDiff * -_cameraRotationSpeed);
	}
}

void mouseButton(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &_mouseX, &_mouseY);
		}
	}
}

void windowResized(GLFWwindow* window, int width, int height) {
	_windowWidth = (width > 0) ? width : 1;
	_windowHeight = (height > 0) ? height : 1;

	const auto ratio = _windowWidth / ayla::Float(_windowHeight);
	_projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);

	glViewport(0, 0, _windowWidth, _windowHeight);
}

void translateCamera(GLFWwindow* window, ayla::Float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		_camera.translate(_camera.getDirection() * deltaTime * _cameraTranslationSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		_camera.translate(_camera.getDirection() * deltaTime * -_cameraTranslationSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		_camera.translate(_camera.getUp() * deltaTime * _cameraTranslationSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		_camera.translate(_camera.getUp() * deltaTime * -_cameraTranslationSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		_camera.translate(_camera.getRight() * deltaTime * _cameraTranslationSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		_camera.translate(_camera.getRight() * deltaTime * -_cameraTranslationSpeed);
	}
}

GLFWwindow* setupGL() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = glfwCreateWindow(_windowWidth, _windowHeight, "Ayla GL - FPS Camera Sample", NULL, NULL);

	if (!window) {
		std::cerr << "Failed to open GLFW window." << std::endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // turn off vsync
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPosCallback(window, mouseMoved);
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetFramebufferSizeCallback(window, windowResized);

	glewExperimental = true; // http://glew.sourceforge.net/basic.html

	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW." << std::endl;
		return NULL;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f); // dark blue background

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // accept fragment if it closer to the camera than the former one

	return window;
}

int main() {
	auto window = setupGL();

	if (!window) {
		return 1;
	}

	const auto model = glm::mat4(1.0f);
	_projection = glm::perspective(glm::radians(45.0f), _windowWidth / ayla::Float(_windowHeight), 0.1f, 100.0f);
	
	const auto sphereRadius = ayla::Float(1.0f);

	const auto sphereMesh = ayla::TriangleMeshBuilder::buildUVSphere(50u, 50u, sphereRadius);
	const ayla_gl::TriangleMeshRenderer sphereMeshGL(*sphereMesh, GL_STATIC_DRAW);

	const ayla_gl::PhongShadingProgram shader;

	do {
		// timing stuff:
		static auto lastTime = ayla::Float(0.0);
		const auto timeNow = ayla::Float(glfwGetTime());
		const auto deltaTime = timeNow - lastTime;
		lastTime = timeNow;

		// update rendering:

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader.getId());

		translateCamera(window, deltaTime);
		_view = _camera.getViewMatrix();

		const auto lightPosition = glm::vec3(-4, 4, 4);
		glUniform3f(shader.getUniform_lightPosition_worldspace(), lightPosition.x, lightPosition.y, lightPosition.z);
		glUniformMatrix4fv(shader.getUniform_M(), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(shader.getUniform_V(), 1, GL_FALSE, &_view[0][0]);
		glUniformMatrix4fv(shader.getUniform_P(), 1, GL_FALSE, &_projection[0][0]);

		glUniform4f(shader.getUniform_materialDiffuseColor(), 0.0f, 0.0f, 1.0f, 1.0f); // blue sphere
		glUniform4f(shader.getUniform_materialSpecularColor(), 1.0f, 1.0f, 1.0f, 1.0f); // high specular
		sphereMeshGL.drawElements();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	glfwTerminate();

	return 0;
}