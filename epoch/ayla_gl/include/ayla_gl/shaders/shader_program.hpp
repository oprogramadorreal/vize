#ifndef AYLA_GL_SHADER_PROGRAM_HPP
#define AYLA_GL_SHADER_PROGRAM_HPP

#include "ayla_gl/config.hpp"

#include "ayla_gl/shaders/shader_source.hpp"

#include <boost/noncopyable.hpp>

namespace ayla_gl {

/**
 * Abstract base class for shader programs.
 *
 * @author O Programador
 */
class AYLA_GL_API ShaderProgram : boost::noncopyable {
protected:
	ShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);
	ShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader, const ShaderSource& tessControlShader, const ShaderSource& tessEvaluationShader);

public:
	virtual ~ShaderProgram();

public:
	inline GLuint getId() const { return _id; }

private:
	GLuint _id = GLuint(0);
};

}

#endif // AYLA_GL_SHADER_PROGRAM_HPP