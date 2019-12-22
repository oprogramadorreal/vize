#include "ayla/image.hpp"

namespace ayla {

Image::Image() = default;

Image::Image(SizeType width, SizeType height, std::vector<Byte>&& rawData)
	: _width(width), _height(height), _bytes(std::move(rawData))
{
	assert(SizeType(_bytes.size()) >= _width * _height);
}

Image::~Image() = default;

SizeType Image::getWidth() const {
	return _width;
}

SizeType Image::getHeight() const {
	return _height;
}

SizeType Image::getNumberOfPixels() const {
	return getWidth() * getHeight();
}

SizeType Image::getBytesPerPixel() const {
	const auto numberOfPixels = getNumberOfPixels();
	
	if (numberOfPixels == SizeType(0)) {
		return SizeType(0);
	}

	return getSizeInBytes() / numberOfPixels;
}

SizeType Image::getSizeInBytes() const {
	return SizeType(_bytes.size());
}

const Image::Byte* Image::getRawData() const {
	return _bytes.data();
}

}