/**
 * Basic example showing how to draw texts using the ayla::TextRenderer abstraction.
 */
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <ayla/directory.hpp>
#include <ayla_gl/renderers/text_renderer.hpp>

#include <glm/gtc/matrix_transform.hpp>

auto _windowWidth = unsigned(1024u);
auto _windowHeight = unsigned(768u);

glm::mat4 _projection;

void windowResized(GLFWwindow* window, int width, int height) {
	_windowWidth = (width > 0) ? width : 1;
	_windowHeight = (height > 0) ? height : 1;

	const auto ratio = _windowWidth / ayla::Float(_windowHeight);
	_projection = glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);

	glViewport(0, 0, _windowWidth, _windowHeight);
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

	auto window = glfwCreateWindow(_windowWidth, _windowHeight, "Ayla GL - Text Renderer Sample", NULL, NULL);

	if (!window) {
		std::cerr << "Failed to open GLFW window." << std::endl;
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); // turn off vsync
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
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

	// create the rendering stuff:

	const auto fontsDirectory = ayla::findDirectory("epoch/ayla_gl/resources/fonts");

	if (fontsDirectory.empty()) {
		std::cerr << "Unable to find fonts directory." << std::endl;
		return 1;
	}

	ayla_gl::TextRenderer text(fontsDirectory + "Holstein.DDS");

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static double lastTime = glfwGetTime(); // glfwGetTime is called only once, the first time this function is called
		static auto framesCount = unsigned(0u);
		static char frameTimeStr[256] = "ms/frame";
		static char fpsStr[256] = "FPS";
		static char framesCountStr[256] = "";

		++framesCount;
		sprintf(framesCountStr, "%d", framesCount);

		if (glfwGetTime() - lastTime >= 1.0) {
			const double frameTime = 1000.0 / double(framesCount);

			sprintf(frameTimeStr, "%.2f", frameTime);
			sprintf(fpsStr, "%.2f", 1000.0 / frameTime);

			framesCount = 0u;
			lastTime += 1.0;
		}

		text.drawText2D(frameTimeStr, -0.9f, -0.9f, 0.05f);
		text.drawText2D(fpsStr, -0.9f, 0.7f, 0.2f);
		text.drawText2D(framesCountStr, -0.4f, 0.0f, 0.2f);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	glfwTerminate();

	return 0;
}