#ifndef VIZE_COLORED_VERTEX_PROGRAM_HPP
#define VIZE_COLORED_VERTEX_PROGRAM_HPP

#include <ayla_gl/shaders/shader_program.hpp>

namespace vize {

/**
 * Sources:
 * - vize/resources/shaders/colored_vertex.vs
 * - vize/resources/shaders/colored_vertex.fs
 *
 * @author O Programador
 */
class ColoredVertexProgram final : public ayla_gl::ShaderProgram {
public:
	ColoredVertexProgram();

public:
	inline GLuint getUniform_M() const { return _M; }
	inline GLuint getUniform_V() const { return _V; }
	inline GLuint getUniform_P() const { return _P; }

private:
	GLuint _M = GLuint(0);
	GLuint _V = GLuint(0);
	GLuint _P = GLuint(0);
};

}

#endif // VIZE_COLORED_VERTEX_PROGRAM_HPP