#include "vize/image/tiff_image.hpp"

#include <QImage>

namespace vize {

TiffImage::TiffImage(const std::string& tiffFile)
	: _tiffFile(tiffFile)
{
	QImage tiff(QString::fromStdString(_tiffFile));

	_width = SizeType(tiff.width());
	_height = SizeType(tiff.height());

	const auto pixelFormat = tiff.pixelFormat();
	const auto bitsPerPixel = pixelFormat.bitsPerPixel();

	const auto pixelSize = SizeType(bitsPerPixel) / 8;

	const auto numberOfPixels = SizeType(_width * _height);
	const auto imageSizeInBytes = SizeType(numberOfPixels * pixelSize);

	_bytes.resize(imageSizeInBytes);
	memcpy(_bytes.data(), tiff.bits(), imageSizeInBytes);
}

}