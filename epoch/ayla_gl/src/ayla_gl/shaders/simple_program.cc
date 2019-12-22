#include <GL/glew.h>

#include "ayla_gl/shaders/simple_program.hpp"
#include "ayla_gl/shaders/default_shaders_directory.hpp"

namespace ayla_gl {

SimpleProgram::SimpleProgram()
	: ShaderProgram(ShaderSource::fromFile(getDefaultShadersDirectoryPath() + "simple.vs"), ShaderSource::fromFile(getDefaultShadersDirectoryPath() + "simple.fs"))
{
	_M = glGetUniformLocation(getId(), "M");
	_V = glGetUniformLocation(getId(), "V");
	_P = glGetUniformLocation(getId(), "P");
	_materialColor = glGetUniformLocation(getId(), "materialColor");
}

}