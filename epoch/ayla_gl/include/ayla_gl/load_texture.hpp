#ifndef AYLA_GL_LOAD_TEXTURE_HPP
#define AYLA_GL_LOAD_TEXTURE_HPP

#include "ayla_gl/config.hpp"

#include <string>

namespace ayla_gl {

/**
 * @return Texture ID.
 */
AYLA_GL_API GLuint loadBMP(const std::string& imagepath);

/**
 * @return Texture ID.
 */
AYLA_GL_API GLuint loadDDS(const std::string& imagepath);

}

#endif // AYLA_GL_LOAD_TEXTURE_HPP