#include <GL/glew.h>

#include "vize/opengl/gl_transfer_function_texture.hpp"

namespace vize {

GLTransferFunctionTexture::GLTransferFunctionTexture() {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_1D, _id);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
}

GLTransferFunctionTexture::~GLTransferFunctionTexture() {
	glDeleteTextures(1, &_id);
}

}