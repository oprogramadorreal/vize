#include <GL/glew.h>

#include "vize/opengl/gl_volume_texture_cache.hpp"
#include "vize/opengl/gl_volume_texture.hpp"

namespace vize {

GLVolumeTextureCache::GLVolumeTextureCache() = default;
GLVolumeTextureCache::~GLVolumeTextureCache() = default;

std::shared_ptr<GLVolumeTexture> GLVolumeTextureCache::getTexture(const Volume& volume) {
	const auto volumeHash = _hashFunction(&volume); // TODO: use volume unique ID
	const auto it = _textures.find(volumeHash);

	std::shared_ptr<GLVolumeTexture> volumeTexture;

	if (it == _textures.end() || it->second.expired()) {
		volumeTexture = std::make_shared<GLVolumeTexture>(volume);
		_textures[volumeHash] = volumeTexture;
	} else {
		volumeTexture = it->second.lock();
	}

	return volumeTexture;
}

}