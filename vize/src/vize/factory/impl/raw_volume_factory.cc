#include "vize/factory/impl/raw_volume_factory.hpp"

#include "vize/model/volume.hpp"

#include <ayla/image.hpp>
#include <fstream>

namespace vize {

namespace {
	std::streampos _calculateFileSize(std::ifstream& file) {
		std::streampos fileSize;
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		return fileSize;
	}
}

std::unique_ptr<ayla::Image> RawVolumeFactory::_createImageImpl(const std::string& imageFile) const {
	std::ifstream file(imageFile, std::ios::binary);

	file.unsetf(std::ios::skipws);

	std::vector<ayla::Image::Byte> rawData;
	rawData.reserve(_calculateFileSize(file));

	rawData.insert(rawData.begin(),
		std::istream_iterator<ayla::Image::Byte>(file),
		std::istream_iterator<ayla::Image::Byte>()
	);

	// TODO: Image size is hard coded. Get this info from a configuration file.
	return std::make_unique<ayla::Image>(512, 512, std::move(rawData));
}

std::unique_ptr<Volume> RawVolumeFactory::_createVolumeImpl(const std::vector<std::string>& rawImages) {
	return _loadVolumeAsRawData(rawImages);
}

}