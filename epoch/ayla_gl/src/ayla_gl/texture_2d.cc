#include <GL/glew.h>

#include "ayla_gl/texture_2d.hpp"

#include <ayla/image.hpp>

namespace ayla_gl {

struct Texture2D::Private {
	GLuint textureId = GLuint(0);
};

Texture2D::Texture2D(const ayla::Image& image, bool repeat)
	: _pimpl(std::make_unique<Private>())
{
	glGenTextures(1, &_pimpl->textureId);
	assert(_pimpl->textureId != GLuint(0));

	glBindTexture(GL_TEXTURE_2D, _pimpl->textureId);

	// @warning assuming this image format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getRawData());

	if (repeat) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_pimpl->textureId);
}

void Texture2D::bind() const {
	glBindTexture(GL_TEXTURE_2D, _pimpl->textureId);
}

}