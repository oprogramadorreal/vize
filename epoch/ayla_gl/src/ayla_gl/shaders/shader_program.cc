#include <GL/glew.h>

#include "ayla_gl/shaders/shader_program.hpp"
#include "ayla_gl/shaders/create_shader_program.hpp"

#include <iostream>

namespace ayla_gl {

ShaderProgram::ShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader) {
	_id = createShaderProgram(vertexShader, fragmentShader);

	if (!_id) {
		std::cerr << "Failed to load shaders." << std::endl;
	}

	assert(_id != 0u);
}

ShaderProgram::ShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader, const ShaderSource& tessControlShader, const ShaderSource& tessEvaluationShader) {
	_id = createShaderProgram(vertexShader, fragmentShader, tessControlShader, tessEvaluationShader);

	if (!_id) {
		std::cerr << "Failed to load shaders." << std::endl;
	}

	assert(_id != 0u);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(_id);
}

}