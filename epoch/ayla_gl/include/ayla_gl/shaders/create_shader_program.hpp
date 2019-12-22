#ifndef AYLA_GL_CREATE_SHADER_PROGRAM_HPP
#define AYLA_GL_CREATE_SHADER_PROGRAM_HPP

#include "ayla_gl/config.hpp"

namespace ayla_gl {

class ShaderSource;

/**
 * @return Program ID.
 */
AYLA_GL_API GLuint createShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader);
AYLA_GL_API GLuint createShaderProgram(const ShaderSource& vertexShader, const ShaderSource& fragmentShader, const ShaderSource& tessellationControlShader, const ShaderSource& tessellationEvaluationShader);

}

#endif // AYLA_GL_CREATE_SHADER_PROGRAM_HPP