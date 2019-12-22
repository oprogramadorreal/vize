#include "ayla/image_cube_map.hpp"
#include "ayla/image.hpp"

namespace ayla {

ImageCubeMap::ImageCubeMap() = default;

ImageCubeMap::ImageCubeMap(
	std::unique_ptr<Image> positiveX,
	std::unique_ptr<Image> negativeX,
	std::unique_ptr<Image> positiveY,
	std::unique_ptr<Image> negativeY,
	std::unique_ptr<Image> positiveZ,
	std::unique_ptr<Image> negativeZ
) {
	_images[0] = std::move(positiveX);
	_images[1] = std::move(negativeX);
	_images[2] = std::move(positiveY);
	_images[3] = std::move(negativeY);
	_images[4] = std::move(positiveZ);
	_images[5] = std::move(negativeZ);
}

ImageCubeMap::~ImageCubeMap() = default;

const Image& ImageCubeMap::getImage(Direction direction) const {
	const auto imageIndex = SizeType(direction);
	assert(imageIndex < SizeType(_images.size()));
	const auto imagePointer = _images[imageIndex].get();
	assert(nullptr != imagePointer);
	return *imagePointer;
}

}