#ifndef LUCCA_QT_CREATE_OPEN_GL_TEXTURE_CUBE_MAP_HPP
#define LUCCA_QT_CREATE_OPEN_GL_TEXTURE_CUBE_MAP_HPP

#include "lucca_qt/config.hpp"

#include <string>

namespace ayla_gl {
	class TextureCubeMap;
}

namespace lucca_qt {

/**
 * @warning Make sure to have set a valid OpenGL context before call this.
 *
 * @see ayla_gl::TextureCubeMap
 */
LUCCA_QT_API std::unique_ptr<ayla_gl::TextureCubeMap> createOpenGLTextureCubeMap(
	const std::string& imageFilePositiveX,
	const std::string& imageFileNegativeX,
	const std::string& imageFilePositiveY,
	const std::string& imageFileNegativeY,
	const std::string& imageFilePositiveZ,
	const std::string& imageFileNegativeZ
);

}

#endif // LUCCA_QT_CREATE_OPEN_GL_TEXTURE_CUBE_MAP_HPP