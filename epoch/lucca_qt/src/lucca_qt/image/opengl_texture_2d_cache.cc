#include "lucca_qt/image/opengl_texture_2d_cache.hpp"

#include "lucca_qt/image/image_cache.hpp"
#include "lucca_qt/image/create_image.hpp"

#include <ayla_gl/texture_2d.hpp>
#include <ayla/image.hpp>

namespace lucca_qt {

OpenGLTexture2DCache::OpenGLTexture2DCache() = default;
OpenGLTexture2DCache::~OpenGLTexture2DCache() = default;

std::shared_ptr<ayla_gl::Texture2D> OpenGLTexture2DCache::getTexture(const std::string& fileName, bool repeating) {
	auto& map = (repeating ? _repeatingTextures : _textures);
	const auto it = map.find(fileName);

	std::shared_ptr<ayla_gl::Texture2D> texture;

	if (it != map.end()) {
		texture = it->second; // get cached texture
	} else {
		std::shared_ptr<ayla::Image> image;

		if (_imageCache) {
			image = _imageCache->getImage(fileName);
		} else {
			image = std::shared_ptr<ayla::Image>(createImage(fileName).release()); // create new image
		}

		if (image) {
			texture = std::shared_ptr<ayla_gl::Texture2D>(
				std::make_unique<ayla_gl::Texture2D>(*image, repeating).release() // create new texture
			);

			map[fileName] = texture; // keep it
		}
	}

	return texture;
}

void OpenGLTexture2DCache::clear() {
	_textures.clear();
	_repeatingTextures.clear();
}

void OpenGLTexture2DCache::setImageCache(ImageCache* imageCache) {
	_imageCache = imageCache;
}

}