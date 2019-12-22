#ifndef LUCCA_QT_CREATE_OPEN_GL_TEXTURE_2D_HPP
#define LUCCA_QT_CREATE_OPEN_GL_TEXTURE_2D_HPP

#include "lucca_qt/config.hpp"

#include <string>

namespace ayla_gl {
	class Texture2D;
}

namespace lucca_qt {

/**
 * @warning Make sure to have set a valid OpenGL context before call this.
 *
 * @see ayla_gl::Texture2D
 */
LUCCA_QT_API std::unique_ptr<ayla_gl::Texture2D> createOpenGLTexture2D(const std::string& imageFile, bool repeat);

}

#endif // LUCCA_QT_CREATE_OPEN_GL_TEXTURE_2D_HPP