#ifndef LUCCA_QT_CREATE_IMAGE_CUBE_MAP_HPP
#define LUCCA_QT_CREATE_IMAGE_CUBE_MAP_HPP

#include "lucca_qt/config.hpp"

#include <string>

namespace ayla {
	class ImageCubeMap;
}

namespace lucca_qt {

/**
 * @see ayla_gl::ImageCubeMap
 */
LUCCA_QT_API std::unique_ptr<ayla::ImageCubeMap> createImageCubeMap(
	const std::string& imageFilePositiveX,
	const std::string& imageFileNegativeX,
	const std::string& imageFilePositiveY,
	const std::string& imageFileNegativeY,
	const std::string& imageFilePositiveZ,
	const std::string& imageFileNegativeZ
);

}

#endif // LUCCA_QT_CREATE_IMAGE_CUBE_MAP_HPP