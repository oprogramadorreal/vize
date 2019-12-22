#include "vize/factory/impl/dicom_volume_factory.hpp"

#include "vize/model/volume.hpp"
#include "vize/image/dicom_image.hpp"

#include <ayla/scaler.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <gdcmIPPSorter.h>

namespace vize {

std::unique_ptr<ayla::Image> DicomVolumeFactory::_createImageImpl(const std::string& imageFile) const {
	return std::make_unique<DicomImage>(imageFile);
}

namespace {
	std::uint16_t _assemble_uint16_t(std::uint8_t lowByte, uint8_t highByte) {
		return static_cast<std::uint16_t>(highByte) << 8 | lowByte;
	}

	bool _findMinMax(const std::vector<std::uint8_t>& bytes, std::uint16_t& minValue, std::uint16_t& maxValue) {
		minValue = std::numeric_limits<std::uint16_t>::max();
		maxValue = std::numeric_limits<std::uint16_t>::min();

		for (auto i = SizeType(0); i < bytes.size(); i += 2) {
			const auto value = _assemble_uint16_t(bytes[i], bytes[i + 1]);

			if (value < minValue) {
				minValue = value;
			}
			if (value > maxValue) {
				maxValue = value;
			}
		}

		return std::numeric_limits<std::uint16_t>::max() != minValue;
	}
}

std::vector<std::string> _sort(const std::vector<std::string>& dicomImages, gdcm::IPPSorter& ippsorter) {
	std::vector<std::string> sortedDicomImages;

	if (!ippsorter.Sort(dicomImages)) {
		return dicomImages; // cannot sort
	}

	return ippsorter.GetFilenames();
}

std::unique_ptr<Volume> DicomVolumeFactory::_createVolumeImpl(const std::vector<std::string>& dicomImages) {
	// sort dicom images
	gdcm::IPPSorter ippsorter; // http://gdcm.sourceforge.net/html/classgdcm_1_1IPPSorter.html
	//ippsorter.SetZSpacingTolerance(0.0001f); // TODO: don't know if this is a good value
	ippsorter.SetComputeZSpacing(true);
	const auto sortedDicomImages = _sort(dicomImages, ippsorter);
	assert(!sortedDicomImages.empty());

	auto volumeWidth = SizeType(0);
	auto volumeHeight = SizeType(0);
	auto volumeDepth = SizeType(0);
	std::vector<std::uint8_t> volumeData;

	if (!_loadVolumeRawData(sortedDicomImages, volumeData, volumeWidth, volumeHeight, volumeDepth)) {
		return nullptr;
	}

	// The following code assumes that:
	// - DICOM image uses 2 bytes per pixel
	// - These two bytes can be assembled in a short integer whose value ranges from 0 to 65535
	//
	// We will put all these values in the range from 0 to 255

	std::uint16_t minValue, maxValue;

	if (!_findMinMax(volumeData, minValue, maxValue)) {
		return nullptr;
	}

	if (maxValue == minValue) {
		minValue = 0;
		maxValue = 1;
	}

	const auto expectedImagePixelSizeInBytes = _getExpectedImagePixelSizeInBytes();
	const ayla::FloatScaler zeroTo255(Float(minValue), Float(maxValue), 0.0f, 255.0f);

	auto volume = std::make_unique<Volume>(volumeWidth, volumeHeight, volumeDepth);
	const auto expectedNumbefOfVoxels = volumeData.size() / expectedImagePixelSizeInBytes;

	if (volume->getNumberOfVoxels() != expectedNumbefOfVoxels) {
		return nullptr; // must not occur
	}

	auto voxelIndex = SizeType(0);

	for (auto i = SizeType(0); i < SizeType(volumeData.size()); i += expectedImagePixelSizeInBytes) {
		const auto value = _assemble_uint16_t(volumeData[i], volumeData[i + 1]);

		const auto scaledValue = std::ceil(
			zeroTo255.scale(Float(value))
		);

		assert(scaledValue >= 0.0f);
		assert(scaledValue <= 255.0f);

		const auto byteValue = std::uint8_t(scaledValue);

		volume->_voxelGrid.setVoxel(voxelIndex++, byteValue);
	}
	
	// compute model matrix
	DicomImage firstImage(sortedDicomImages.front());
	DicomImage lastImage(sortedDicomImages.back());
	const auto xySpacing = firstImage.getPixelSpacing() * glm::vec2(firstImage.getWidth(), firstImage.getHeight());
	const auto zSpacing = (lastImage.getSlicePosition() - firstImage.getSlicePosition());
	const auto volumeProportions = glm::vec3(xySpacing, zSpacing) / Float(sortedDicomImages.size());

	const auto R = glm::rotate(glm::mat4(1), glm::radians(-90.0f), glm::vec3(1, 0, 0));
	const auto RS = glm::scale(R, volumeProportions);
	volume->setModelMatrix(RS);

	return std::move(volume);
}

}