#ifndef VIZE_TIFF_IMAGE_HPP
#define VIZE_TIFF_IMAGE_HPP

#include "vize/config.hpp"

#include <ayla/image.hpp>

#include <string>

namespace vize {

/**
 * @author O Programador
 */
class TiffImage final : public ayla::Image {
public:
	TiffImage(const std::string& tiffFile);

private:
	const std::string _tiffFile;
};

}

#endif // VIZE_TIFF_IMAGE_HPP