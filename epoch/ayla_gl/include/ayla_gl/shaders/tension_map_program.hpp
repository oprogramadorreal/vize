#ifndef AYLA_GL_TENSION_MAP_PROGRAM_HPP
#define AYLA_GL_TENSION_MAP_PROGRAM_HPP

#include "ayla_gl/config.hpp"
#include "ayla_gl/shaders/shader_program.hpp"

namespace ayla_gl {

/**
 * Sources:
 * - ayla_gl/shaders/tension_map.vs
 * - ayla_gl/shaders/tension_map.fs
 *
 * @author O Programador
 */
class AYLA_GL_API TensionMapProgram final : public ShaderProgram {
public:
	TensionMapProgram();

public:
	inline GLuint getUniform_M() const { return _M; }
	inline GLuint getUniform_V() const { return _V; }
	inline GLuint getUniform_P() const { return _P; }
	inline GLuint getUniform_minTension() const { return _minTension; }
	inline GLuint getUniform_maxTension() const { return _maxTension; }

private:
	GLuint _M = GLuint(0);
	GLuint _V = GLuint(0);
	GLuint _P = GLuint(0);
	GLuint _minTension = GLuint(0);
	GLuint _maxTension = GLuint(0);
};

}

#endif // AYLA_GL_TENSION_MAP_PROGRAM_HPP