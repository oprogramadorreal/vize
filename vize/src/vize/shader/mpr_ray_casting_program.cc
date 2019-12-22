#include <GL/glew.h>

#include "vize/shader/mpr_ray_casting_program.hpp"

#include <lucca_qt/read_shader_resource.hpp>

namespace vize {

MprRayCastingProgram::MprRayCastingProgram()
	: ShaderProgram(
		lucca_qt::readShaderResource(":resources/shaders/mpr_ray_casting.vs"),
		lucca_qt::readShaderResource(":resources/shaders/mpr_ray_casting.fs")
	)
{
	_cameraPosition_normmodelspace = glGetUniformLocation(getId(), "cameraPosition_normmodelspace");
	_lightPosition_normmodelspace = glGetUniformLocation(getId(), "lightPosition_normmodelspace");
	_volumeSampler = glGetUniformLocation(getId(), "volumeSampler");
	_transferFunction = glGetUniformLocation(getId(), "transferFunction");
	_raysOriginTexture = glGetUniformLocation(getId(), "raysOriginTexture");
	_raysEndTexture = glGetUniformLocation(getId(), "raysEndTexture");
	_xPlanePosition = glGetUniformLocation(getId(), "xPlanePosition");
	_yPlanePosition = glGetUniformLocation(getId(), "yPlanePosition");
	_zPlanePosition = glGetUniformLocation(getId(), "zPlanePosition");
	_lightingEnabled = glGetUniformLocation(getId(), "lightingEnabled");
	_cuttingPlanes = glGetUniformLocation(getId(), "cuttingPlanes");
	_volumeSpacing = glGetUniformLocation(getId(), "volumeSpacing");
	_sampleStep = glGetUniformLocation(getId(), "sampleStep");
	_backgroundColor = glGetUniformLocation(getId(), "backgroundColor");
}

}