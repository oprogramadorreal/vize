#ifndef AYLA_GL_SIMPLE_PROGRAM_HPP
#define AYLA_GL_SIMPLE_PROGRAM_HPP

#include "ayla_gl/config.hpp"
#include "ayla_gl/shaders/shader_program.hpp"

namespace ayla_gl {

/**
 * Sources:
 * - ayla_gl/shaders/simple.vs
 * - ayla_gl/shaders/simple.fs
 *
 * @author O Programador
 */
class AYLA_GL_API SimpleProgram final : public ShaderProgram {
public:
	SimpleProgram();

public:
	inline GLuint getUniform_M() const { return _M; }
	inline GLuint getUniform_V() const { return _V; }
	inline GLuint getUniform_P() const { return _P; }
	inline GLuint getUniform_materialColor() const { return _materialColor; }

private:
	GLuint _M = GLuint(0);
	GLuint _V = GLuint(0);
	GLuint _P = GLuint(0);
	GLuint _materialColor = GLuint(0);
};

}

#endif // AYLA_GL_SIMPLE_PROGRAM_HPP