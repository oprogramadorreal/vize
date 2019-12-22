#include "vize/factory/impl/tiff_volume_factory.hpp"

#include "vize/model/volume.hpp"
#include "vize/image/tiff_image.hpp"

namespace vize {

std::unique_ptr<ayla::Image> TiffVolumeFactory::_createImageImpl(const std::string& imageFile) const {
	return std::make_unique<TiffImage>(imageFile);
}

std::unique_ptr<Volume> TiffVolumeFactory::_createVolumeImpl(const std::vector<std::string>& tiffImages) {
	return _loadVolumeAsRawData(tiffImages);
}

}