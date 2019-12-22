#include "lucca_qt/image/create_image_cube_map.hpp"

#include "lucca_qt/image/create_image.hpp"

#include <ayla/image.hpp>
#include <ayla/image_cube_map.hpp>

#include <array>

namespace lucca_qt {

std::unique_ptr<ayla::ImageCubeMap> createImageCubeMap(
	const std::string& imageFilePositiveX,
	const std::string& imageFileNegativeX,
	const std::string& imageFilePositiveY,
	const std::string& imageFileNegativeY,
	const std::string& imageFilePositiveZ,
	const std::string& imageFileNegativeZ
) {
	const std::array<std::string, 6> imagesFiles = {
		imageFilePositiveX, imageFileNegativeX,
		imageFilePositiveY, imageFileNegativeY,
		imageFilePositiveZ, imageFileNegativeZ
	};

	std::array<std::unique_ptr<ayla::Image>, 6> images;

	for (auto i = SizeType(0); i < SizeType(6); ++i) {
		if (auto newImage = createImage(imagesFiles[i])) {
			images[i] = std::move(newImage);
		} else {
			return nullptr;
		}
	}

	return std::make_unique<ayla::ImageCubeMap>(
		std::move(images[0]),
		std::move(images[1]),
		std::move(images[2]),
		std::move(images[3]),
		std::move(images[4]),
		std::move(images[5])
	);
}

}