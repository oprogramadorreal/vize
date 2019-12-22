#ifndef AYLA_GL_TEXTURE_CUBE_MAP_HPP
#define AYLA_GL_TEXTURE_CUBE_MAP_HPP

#include "ayla_gl/config.hpp"

#include <boost/noncopyable.hpp>

namespace ayla {
	class Image;
	class ImageCubeMap;
}

namespace ayla_gl {

/**
 * @see ayla::Image
 *
 * @author O Programador
 */
class AYLA_GL_API TextureCubeMap final : boost::noncopyable {
public:
	TextureCubeMap(const ayla::ImageCubeMap& image);

	TextureCubeMap(
		const ayla::Image& positiveX,
		const ayla::Image& negativeX,
		const ayla::Image& positiveY,
		const ayla::Image& negativeY,
		const ayla::Image& positiveZ,
		const ayla::Image& negativeZ
	);

	~TextureCubeMap();

	void bind() const;

private:
	struct Private;
	std::unique_ptr<Private> _pimpl;
};

}

#endif // AYLA_GL_TEXTURE_CUBE_MAP_HPP