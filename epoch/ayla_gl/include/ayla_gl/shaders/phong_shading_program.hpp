#ifndef AYLA_GL_PHONG_SHADING_PROGRAM_HPP
#define AYLA_GL_PHONG_SHADING_PROGRAM_HPP

#include "ayla_gl/config.hpp"
#include "ayla_gl/shaders/shader_program.hpp"

namespace ayla_gl {

/**
 * Sources:
 * - ayla_gl/shaders/phong_shading.vs
 * - ayla_gl/shaders/phong_shading.fs
 *
 * @author O Programador
 */
class AYLA_GL_API PhongShadingProgram final : public ShaderProgram {
public:
	PhongShadingProgram();

public:
	inline GLuint getUniform_M() const { return _M; }
	inline GLuint getUniform_V() const { return _V; }
	inline GLuint getUniform_P() const { return _P; }
	inline GLuint getUniform_lightPosition_worldspace() const { return _lightPosition_worldspace; }
	inline GLuint getUniform_lightPower() const { return _lightPower; }
	inline GLuint getUniform_lightQuadraticAttenuation() const { return _lightQuadraticAttenuation; }
	inline GLuint getUniform_materialDiffuseColor() const { return _materialDiffuseColor; }
	inline GLuint getUniform_materialSpecularColor() const { return _materialSpecularColor; }
	inline GLuint getUniform_materialAmbientColor() const { return _materialAmbientColor; }
	inline GLuint getUniform_textureEnabled() const { return _textureEnabled; }
	inline GLuint getUniform_textureSampler() const { return _textureSampler; }

private:
	GLuint _M = GLuint(0);
	GLuint _V = GLuint(0);
	GLuint _P = GLuint(0);
	GLuint _lightPosition_worldspace = GLuint(0);
	GLuint _lightPower = GLuint(0);
	GLuint _lightQuadraticAttenuation = GLuint(0);
	GLuint _materialDiffuseColor = GLuint(0);
	GLuint _materialSpecularColor = GLuint(0);
	GLuint _materialAmbientColor = GLuint(0);
	GLuint _textureEnabled = GLuint(0);
	GLuint _textureSampler = GLuint(0);
};

}

#endif // AYLA_GL_PHONG_SHADING_PROGRAM_HPP