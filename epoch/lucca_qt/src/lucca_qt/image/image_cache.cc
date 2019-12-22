#include "lucca_qt/image/image_cache.hpp"
#include "lucca_qt/image/create_image.hpp"

#include <ayla/image.hpp>

namespace lucca_qt {

ImageCache::ImageCache() = default;
ImageCache::~ImageCache() = default;

std::shared_ptr<ayla::Image> ImageCache::getImage(const std::string& fileName) {
	const auto it = _images.find(fileName);

	std::shared_ptr<ayla::Image> image;

	if (it != _images.end()) {
		image = it->second; // get cached image
	} else {
		image = std::shared_ptr<ayla::Image>(createImage(fileName).release()); // create new image

		if (image) {
			_images[fileName] = image; // keep it
		}
	}

	return image;
}

}