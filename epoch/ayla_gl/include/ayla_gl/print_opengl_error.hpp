#ifndef AYLA_GL_PRINT_OPENGL_ERROR_HPP
#define AYLA_GL_PRINT_OPENGL_ERROR_HPP

#include "ayla_gl/config.hpp"

namespace ayla_gl {

AYLA_GL_API bool _printOpenGLError(const char *file, int line);

/**
 * Check and print OpenGL errors.
 *
 * Returns TRUE if an OpenGL error occurred, FALSE otherwise.
 */
#define printOpenGLError() _printOpenGLError(__FILE__, __LINE__)

/**
 * Check OpenGL errors.
 *
 * Returns TRUE if an OpenGL error occurred, FALSE otherwise.
 */
AYLA_GL_API bool checkOpenGLError();

}

#endif // AYLA_GL_PRINT_OPENGL_ERROR_HPP