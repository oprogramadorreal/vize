#ifndef LUCCA_QT_OPENGL_TEXTURE_2D_CACHE_HPP
#define LUCCA_QT_OPENGL_TEXTURE_2D_CACHE_HPP

#include "lucca_qt/config.hpp"

#include <boost/noncopyable.hpp>

#include <unordered_map>

namespace ayla_gl {
	class Texture2D;
}

namespace lucca_qt {

class ImageCache;

/**
 * @author O Programador
 */
class LUCCA_QT_API OpenGLTexture2DCache final : boost::noncopyable {
public:
	OpenGLTexture2DCache();
	~OpenGLTexture2DCache();

	/**
	 * @warning Make sure to have set a valid OpenGL context before call this.
	 */
	std::shared_ptr<ayla_gl::Texture2D> getTexture(const std::string& fileName, bool repeating);

	/**
	 * Delete all OpenGL textures in this cache.
	 *
	 * @warning Make sure to have set a valid OpenGL context before call this.
	 */
	void clear();

	void setImageCache(ImageCache* imageCache);

private:
	using MapType = std::unordered_map<std::string, std::shared_ptr<ayla_gl::Texture2D> >;

	MapType _textures;
	MapType _repeatingTextures;

	ImageCache* _imageCache = nullptr;
};

}

#endif // LUCCA_QT_OPENGL_TEXTURE_2D_CACHE_HPP