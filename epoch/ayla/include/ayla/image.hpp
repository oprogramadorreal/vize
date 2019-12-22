#ifndef AYLA_IMAGE_HPP
#define AYLA_IMAGE_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/image_serializer.hpp"

#include <vector>

namespace ayla {

/**
 * @see ayla_gl::Texture2D
 *
 * @author O Programador
 */
class AYLA_API Image {
public:
	/**
	 * Image byte type.
	 */
	using Byte = std::uint8_t;
	static_assert(sizeof(Byte) == 1, "ayla_gl::Image::Byte type must be 1 byte long!");

protected:
	Image();

public:
	Image(SizeType width, SizeType height, std::vector<Byte>&& rawData);
	virtual ~Image();

	/**
	 * @return The width of this image in pixels.
	 */
	SizeType getWidth() const;

	/**
	 * @return The height of this image in pixels.
	 */
	SizeType getHeight() const;

	/**
	 * @return Number of pixels in this image.
	 * The same as Image::getWidth() * Image::getHeight().
	 */
	SizeType getNumberOfPixels() const;

	/**
	 * @return Image pixel size in bytes.
	 */
	SizeType getBytesPerPixel() const;

	/**
	 * @return The size in bytes of this image.
	 */
	SizeType getSizeInBytes() const;

	/**
	 * @return A pointer to the first byte of this image data.
	 */
	const Byte* getRawData() const;

protected:
	SizeType _width = SizeType(0);
	SizeType _height = SizeType(0);

	std::vector<Byte> _bytes;

	template <class Archive> friend void boost::serialization::serialize(Archive&, ayla::Image&, const unsigned int);
	template <class Archive> friend void boost::serialization::load_construct_data(Archive&, ayla::Image*, const unsigned int);
};

}

#endif // AYLA_IMAGE_HPP