#include "vize/factory/volume_factory.hpp"

#include "vize/model/volume.hpp"

#include <ayla/image.hpp>
#include <ayla/directory.hpp>

#include <iostream>

namespace vize {

VolumeFactory::VolumeFactory() = default;
VolumeFactory::~VolumeFactory() = default;

void VolumeFactory::connectAsync(lucca_qt::Async& async) {
	const auto cancelTask = [this]() {
		_asyncCanceled = true;
		AYLA_DEBUG_MESSAGE("Building canceled!");
	};

	_asyncConnections.push_back(async.connectToTaskStoppedSignal(cancelTask));
	_asyncConnections.push_back(async.listenToTaskProgressSignal(_progressSignal));
}

SizeType VolumeFactory::_getExpectedImagePixelSizeInBytes() const {
	return _expectedImagePixelSizeInBytes ? *_expectedImagePixelSizeInBytes : SizeType(0);
}

std::unique_ptr<ayla::Image> VolumeFactory::_createImage(const std::string& imageFile) {
	auto image = _createImageImpl(imageFile);

	if (image) {
		if (!_expectedImagePixelSizeInBytes) { // if this is the first image
			_expectedImagePixelSizeInBytes = image->getBytesPerPixel();
		} else {
			if (image->getBytesPerPixel() != _expectedImagePixelSizeInBytes) { // all following images must have the same pixel size
				return nullptr; // cannot handle this image
			}
		}
	}

	return std::move(image);
}

bool VolumeFactory::_loadVolumeRawData(const std::vector<std::string>& allImages, std::vector<std::uint8_t>& volumeData, SizeType& volumeWidth, SizeType& volumeHeight, SizeType& volumeDepth) {
	if (allImages.empty()) {
		return false; // nothing to load
	}

	_expectedImagePixelSizeInBytes = boost::none;
	const auto firstImage = _createImage(allImages.front());

	if (!firstImage) {
		return false; // cannot load first image
	}

	assert(firstImage->getBytesPerPixel() == _getExpectedImagePixelSizeInBytes());

	volumeWidth = firstImage->getWidth();
	volumeHeight = firstImage->getHeight();
	volumeDepth = SizeType(allImages.size());

	// all images in volume must have the same size
	const auto imageSize = firstImage->getSizeInBytes();
	
	volumeData.resize(imageSize * volumeDepth);
	auto currentVolumeSize = SizeType(0);

	_asyncCanceled = false;

	for (const auto& i : allImages) {
		if (_asyncCanceled) {
			return false;
		}

		const auto image = _createImage(i);

		if (!image) {
			return false; // cannot load current image
		}

		if (image->getSizeInBytes() != imageSize) {
			return false; // all images must have the same size
		}

		// accumulate current image data in volumeData
		memcpy(volumeData.data() + currentVolumeSize, image->getRawData(), imageSize);
		currentVolumeSize += imageSize;

		const auto progress = SizeType((currentVolumeSize / Float(volumeData.size())) * Float(100.0f));
		_progressSignal(progress);
	}

	return true;
}

std::unique_ptr<Volume> VolumeFactory::_loadVolumeAsRawData(const std::vector<std::string>& allImages) {
	auto volumeWidth = SizeType(0);
	auto volumeHeight = SizeType(0);
	auto volumeDepth = SizeType(0);

	std::vector<std::uint8_t> volumeData;

	if (!_loadVolumeRawData(allImages, volumeData, volumeWidth, volumeHeight, volumeDepth)) {
		return nullptr;
	}

	const auto expectedImagePixelSizeInBytes = _getExpectedImagePixelSizeInBytes();

	auto volume = std::make_unique<Volume>(volumeWidth, volumeHeight, volumeDepth);
	const auto expectedNumbefOfVoxels = volumeData.size() / expectedImagePixelSizeInBytes;

	if (volume->getNumberOfVoxels() != expectedNumbefOfVoxels) {
		return nullptr; // must not occur
	}

	auto voxelIndex = SizeType(0);

	for (auto i = SizeType(0); i < SizeType(volumeData.size()); i += expectedImagePixelSizeInBytes) {
		volume->_voxelGrid.setVoxel(voxelIndex++, volumeData[i]);
	}

	return std::move(volume);
}

std::unique_ptr<Volume> VolumeFactory::createVolume(const std::vector<std::string>& images) {
	return _createVolumeImpl(images);
}

std::unique_ptr<Volume> VolumeFactory::createVolumeFromDirectory(const std::string& imagesDirectory) {
	return createVolume(ayla::getFilesPathInDirectory(imagesDirectory));
}

}