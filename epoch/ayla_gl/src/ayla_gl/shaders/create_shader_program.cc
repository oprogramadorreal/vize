#include <GL/glew.h>

#include "ayla_gl/shaders/create_shader_program.hpp"
#include "ayla_gl/shaders/shader_source.hpp"

#include <vector>

namespace ayla_gl {
	
namespace {

void _checkShaderProgram(GLuint programID) {
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}
}

GLuint _loadShader(const std::string& shaderCode, GLenum shaderType) {
	const auto shaderID = glCreateShader(shaderType);

	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);

	// Check Shader
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	return shaderID;
}

}

GLuint createShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader) {
	const auto vertexShaderID = _loadShader(vertexShader.getSourceCode(), GL_VERTEX_SHADER);
	const auto fragmentShaderID = _loadShader(fragmentShader.getSourceCode(), GL_FRAGMENT_SHADER);

	const auto programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	_checkShaderProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

GLuint createShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader, const ShaderSource& tessellationControlShader, const ShaderSource& tessellationEvaluationShader) {
	const auto vertexShaderID = _loadShader(vertexShader.getSourceCode(), GL_VERTEX_SHADER);
	const auto fragmentShaderID = _loadShader(fragmentShader.getSourceCode(), GL_FRAGMENT_SHADER);
	const auto tessellationControlShaderID = _loadShader(tessellationControlShader.getSourceCode(), GL_TESS_CONTROL_SHADER);
	const auto tessellationEvaluationShaderID = _loadShader(tessellationEvaluationShader.getSourceCode(), GL_TESS_EVALUATION_SHADER);

	const auto programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glAttachShader(programID, tessellationControlShaderID);
	glAttachShader(programID, tessellationEvaluationShaderID);
	glLinkProgram(programID);

	_checkShaderProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	glDeleteShader(tessellationControlShaderID);
	glDeleteShader(tessellationEvaluationShaderID);

	return programID;
}

}