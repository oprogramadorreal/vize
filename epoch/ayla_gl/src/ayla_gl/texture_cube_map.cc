#include <GL/glew.h>

#include "ayla_gl/texture_cube_map.hpp"

#include <ayla/image.hpp>
#include <ayla/image_cube_map.hpp>

namespace ayla_gl {

struct TextureCubeMap::Private {
	GLuint textureId = GLuint(0);
};

TextureCubeMap::TextureCubeMap(const ayla::ImageCubeMap& image)
	: TextureCubeMap(
		image.getImage(ayla::ImageCubeMap::Direction::PositiveX),
		image.getImage(ayla::ImageCubeMap::Direction::NegativeX),
		image.getImage(ayla::ImageCubeMap::Direction::PositiveY),
		image.getImage(ayla::ImageCubeMap::Direction::NegativeY),
		image.getImage(ayla::ImageCubeMap::Direction::PositiveZ),
		image.getImage(ayla::ImageCubeMap::Direction::NegativeZ)
	)
{ }

TextureCubeMap::TextureCubeMap(
	const ayla::Image& positiveX,
	const ayla::Image& negativeX,
	const ayla::Image& positiveY,
	const ayla::Image& negativeY,
	const ayla::Image& positiveZ,
	const ayla::Image& negativeZ
)
	: _pimpl(std::make_unique<Private>())
{
	glGenTextures(1, &_pimpl->textureId);
	assert(_pimpl->textureId != GLuint(0));

	glBindTexture(GL_TEXTURE_CUBE_MAP, _pimpl->textureId);

	// @warning assuming this image format
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, positiveX.getWidth(), positiveX.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, positiveX.getRawData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, negativeX.getWidth(), negativeX.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negativeX.getRawData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, positiveY.getWidth(), positiveY.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, positiveY.getRawData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, negativeY.getWidth(), negativeY.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negativeY.getRawData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, positiveZ.getWidth(), positiveZ.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, positiveZ.getRawData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, negativeZ.getWidth(), negativeZ.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negativeZ.getRawData());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureCubeMap::~TextureCubeMap() {
	glDeleteTextures(1, &_pimpl->textureId);
}

void TextureCubeMap::bind() const {
	glBindTexture(GL_TEXTURE_CUBE_MAP, _pimpl->textureId);
}

}