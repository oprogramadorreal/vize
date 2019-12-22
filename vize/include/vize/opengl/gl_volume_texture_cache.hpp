#ifndef VIZE_GL_VOLUME_TEXTURE_CACHE_HPP
#define VIZE_GL_VOLUME_TEXTURE_CACHE_HPP

#include "vize/config.hpp"

#include <unordered_map>

namespace vize {

class Volume;
class GLVolumeTexture;

/**
 * @author O Programador
 */
class GLVolumeTextureCache final {
public:
	GLVolumeTextureCache();
	~GLVolumeTextureCache();

	/**
	 * @warning Make sure OpenGL context is properly bound before calling this method.
	 */
	std::shared_ptr<GLVolumeTexture> getTexture(const Volume& volume);

private:
	using MapType = std::unordered_map<std::size_t, std::weak_ptr<GLVolumeTexture> >;
	using HashFunctionType = std::hash<const Volume*>;

	MapType _textures;
	HashFunctionType _hashFunction;
};

}

#endif // VIZE_GL_VOLUME_TEXTURE_CACHE_HPP