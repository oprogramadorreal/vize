#include "ayla_gl/print_opengl_error.hpp"

#include <GL/glew.h>
#include <iostream>
#include <string>

namespace ayla_gl {

namespace {

std::string _getErrorString(GLenum glError) {
	switch (glError) {
	case GL_NO_ERROR: return "GL_NO_ERROR";
	case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
	case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
	case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
	case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
	default:;
	}

	return "Unknown error!";
}

}

bool _printOpenGLError(const char* file, int line) {
	const GLenum glError = glGetError();

	if (glError == GL_NO_ERROR) {
		return false;
	}

	std::cerr << "glError in file " << std::string(file) << " at line " << line << ": " << _getErrorString(glError) << std::endl;

	return true;
}

bool checkOpenGLError() { return glGetError() != GL_NO_ERROR; }

}