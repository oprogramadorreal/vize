#include <GL/glew.h>

#include "ayla_gl/shaders/phong_shading_program.hpp"
#include "ayla_gl/shaders/default_shaders_directory.hpp"

namespace ayla_gl {

PhongShadingProgram::PhongShadingProgram()
	: ShaderProgram(ShaderSource::fromFile(getDefaultShadersDirectoryPath() + "phong_shading.vs"), ShaderSource::fromFile(getDefaultShadersDirectoryPath() + "phong_shading.fs"))
{
	_M = glGetUniformLocation(getId(), "M");
	_V = glGetUniformLocation(getId(), "V");
	_P = glGetUniformLocation(getId(), "P");
	_lightPosition_worldspace = glGetUniformLocation(getId(), "lightPosition_worldspace");
	_lightPower = glGetUniformLocation(getId(), "lightPower");
	_lightQuadraticAttenuation = glGetUniformLocation(getId(), "lightQuadraticAttenuation");
	_materialDiffuseColor = glGetUniformLocation(getId(), "materialDiffuseColor");
	_materialSpecularColor = glGetUniformLocation(getId(), "materialSpecularColor");
	_materialAmbientColor = glGetUniformLocation(getId(), "materialAmbientColor");
	_textureEnabled = glGetUniformLocation(getId(), "textureEnabled");
	_textureSampler = glGetUniformLocation(getId(), "textureSampler");
}

}