#ifndef VIZE_MPR_RAY_CASTING_PROGRAM_HPP
#define VIZE_MPR_RAY_CASTING_PROGRAM_HPP

#include <ayla_gl/shaders/shader_program.hpp>

namespace vize {

/**
 * Sources:
 * - vize/resources/shaders/mpr_ray_casting.vs
 * - vize/resources/shaders/mpr_ray_casting.fs
 *
 * @author O Programador
 */
class MprRayCastingProgram final : public ayla_gl::ShaderProgram {
public:
	MprRayCastingProgram();

public:
	inline GLuint getUniform_cameraPosition_normmodelspace() const { return _cameraPosition_normmodelspace; }
	inline GLuint getUniform_lightPosition_normmodelspace() const { return _lightPosition_normmodelspace; }
	inline GLuint getUniform_volumeSampler() const { return _volumeSampler; }
	inline GLuint getUniform_transferFunction() const { return _transferFunction; }
	inline GLuint getUniform_raysOriginTexture() const { return _raysOriginTexture; }
	inline GLuint getUniform_raysEndTexture() const { return _raysEndTexture; }
	inline GLuint getUniform_xPlanePosition() const { return _xPlanePosition; }
	inline GLuint getUniform_yPlanePosition() const { return _yPlanePosition; }
	inline GLuint getUniform_zPlanePosition() const { return _zPlanePosition; }
	inline GLuint getUniform_lightingEnabled() const { return _lightingEnabled; }
	inline GLuint getUniform_cuttingPlanes() const { return _cuttingPlanes; }
	inline GLuint getUniform_volumeSpacing() const { return _volumeSpacing; }
	inline GLuint getUniform_sampleStep() const { return _sampleStep; }
	inline GLuint getUniform_backgroundColor() const { return _backgroundColor; }

private:
	GLuint _cameraPosition_normmodelspace = GLuint(0);
	GLuint _lightPosition_normmodelspace = GLuint(0);
	GLuint _volumeSampler = GLuint(0);
	GLuint _transferFunction = GLuint(0);
	GLuint _raysOriginTexture = GLuint(0);
	GLuint _raysEndTexture = GLuint(0);
	GLuint _xPlanePosition = GLuint(0);
	GLuint _yPlanePosition = GLuint(0);
	GLuint _zPlanePosition = GLuint(0);
	GLuint _lightingEnabled = GLuint(0);
	GLuint _cuttingPlanes = GLuint(0);
	GLuint _volumeSpacing = GLuint(0);
	GLuint _sampleStep = GLuint(0);
	GLuint _backgroundColor = GLuint(0);
};

}

#endif // VIZE_MPR_RAY_CASTING_PROGRAM_HPP