#ifndef VIZE_DICOM_IMAGE_HPP
#define VIZE_DICOM_IMAGE_HPP

#include "vize/config.hpp"

#include <ayla/image.hpp>

#include <string>

namespace vize {

/**
 * @author O Programador
 */
class DicomImage final : public ayla::Image {
public:
	DicomImage(const std::string& dicomFile);
	virtual ~DicomImage();

	glm::vec2 getPixelSpacing();

	Float getSlicePosition();

private:
	const std::string _dicomFile;

	struct Private;
	std::unique_ptr<Private> _pimpl;
};

}

#endif // VIZE_DICOM_IMAGE_HPP