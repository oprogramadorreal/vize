#include "vize/image/dicom_image.hpp"

#include <gdcmImageReader.h>
#include <gdcmPhotometricInterpretation.h>

namespace vize {

struct DicomImage::Private {
	gdcm::ImageReader reader;
};

DicomImage::DicomImage(const std::string& dicomFile)
	: _dicomFile(dicomFile), _pimpl(std::make_unique<Private>())
{
	_pimpl->reader.SetFileName(_dicomFile.c_str());

	if (!_pimpl->reader.Read()) {
		return; // could not read dicomImage
	}

	const auto& gdcmImage = _pimpl->reader.GetImage();

	if (gdcmImage.GetNumberOfDimensions() < 2) {
		return; // could not read image with less than 2 dimensions
	}

	//std::cout << gdcmImage.GetPhotometricInterpretation() << std::endl; // expecting MONOCHROME2
	//std::cout << gdcmImage.GetPixelFormat() << std::endl;

	// not sure if this is correct:
	const auto pixelSize = SizeType(gdcmImage.GetPixelFormat().GetBitsAllocated() / 8);

	_width = gdcmImage.GetDimensions()[0];
	_height = gdcmImage.GetDimensions()[1];

	const auto numberOfPixels = SizeType(_width * _height);
	const auto imageSizeInBytes = SizeType(numberOfPixels * pixelSize);

	if (SizeType(gdcmImage.GetBufferLength()) != imageSizeInBytes) {
		return; // image buffer does not have the expected size
	}

	_bytes.resize(imageSizeInBytes);

	if (!gdcmImage.GetBuffer((char*)_bytes.data())) {
		_bytes.clear(); // could not read image buffer
	}
}

DicomImage::~DicomImage() = default;

glm::vec2 DicomImage::getPixelSpacing() {
	const auto& gdcmImage = _pimpl->reader.GetImage();

	return glm::vec2(
		Float(gdcmImage.GetSpacing(0)),
		Float(gdcmImage.GetSpacing(1))
	);
}

Float DicomImage::getSlicePosition() {
	return Float(_pimpl->reader.GetImage().GetOrigin(2));
}

}