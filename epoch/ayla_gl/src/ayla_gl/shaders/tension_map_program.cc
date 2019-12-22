#include <GL/glew.h>

#include "ayla_gl/shaders/tension_map_program.hpp"
#include "ayla_gl/shaders/default_shaders_directory.hpp"

namespace ayla_gl {

TensionMapProgram::TensionMapProgram()
	: ShaderProgram(ShaderSource::fromFile(getDefaultShadersDirectoryPath() + "tension_map.vs"), ShaderSource::fromFile(getDefaultShadersDirectoryPath() + "tension_map.fs"))
{
	_M = glGetUniformLocation(getId(), "M");
	_V = glGetUniformLocation(getId(), "V");
	_P = glGetUniformLocation(getId(), "P");
	_minTension = glGetUniformLocation(getId(), "minTension");
	_maxTension = glGetUniformLocation(getId(), "maxTension");
}

}