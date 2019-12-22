#ifndef VIZE_COLOR_HPP
#define VIZE_COLOR_HPP

#include "vize/config.hpp"

#include "vize/serialization/color_serializer.hpp"

namespace vize {
	
/**
 * Red, green, blue and alpha (RGBA) color abstraction.
 *
 * Each channel is stored in a 8-bit unsigned integer.
 * Thus each channel value must vary from 0 to 255.
 *
 * @see https://en.wikipedia.org/wiki/RGBA_color_space
 *
 * @author O Programador
 */
class Color final {
public:
	using ChannelType = std::uint8_t;

public:
	Color();

	/**
	 * @param r The value of the red channel of this color.
	 * @param g The value of the green channel of this color.
	 * @param b The value of the blue channel of this color.
	 * @param a The value of the alpha channel of this color.
	 */
	Color(ChannelType r, ChannelType g, ChannelType b, ChannelType a = ChannelType(255));

public:
	/**
	 * @return The value of the red channel of this color. The value ranges from 0 to 255.
	 */
	ChannelType getR() const;

	/**
	 * @return The value of the green channel of this color. The value ranges from 0 to 255.
	 */
	ChannelType getG() const;

	/**
	 * @return The value of the blue channel of this color. The value ranges from 0 to 255.
	 */
	ChannelType getB() const;

	/**
	 * @return The value of the alpha channel of this color. The value ranges from 0 to 255.
	 */
	ChannelType getA() const;

public:
	/**
	 * @return The value of the red channel of this color. The value is a vize::Float ranging from 0 to 1.
	 */
	Float getNormalizedR() const;

	/**
	 * @return The value of the green channel of this color. The value is a vize::Float ranging from 0 to 1.
	 */
	Float getNormalizedG() const;

	/**
	 * @return The value of blue blue channel of this color. The value is a vize::Float ranging from 0 to 1.
	 */
	Float getNormalizedB() const;

	/**
	 * @return The value of the alpha channel of this color. The value is a vize::Float ranging from 0 to 1.
	 */
	Float getNormalizedA() const;

public:
	/**
	 * Add this color to @param other color.
	 */
	Color operator+(const Color& other) const;

	/**
	 * Multiply this color by a scalar @param value.
	 */
	Color operator*(Float value) const;

public:
	/**
	 * @return A representation of the white color.
	 */
	static Color white();

	/**
	 * @return A representation of the black color.
	 */
	static Color black();

	/**
	 * @return A representation of the red color.
	 */
	static Color red();

	/**
	 * @return A representation of the green color.
	 */
	static Color green();

	/**
	 * @return A representation of the blue color.
	 */
	static Color blue();

private:
	ChannelType _r; // red channel
	ChannelType _g; // green channel
	ChannelType _b; // blue channel
	ChannelType _a; // alpha channel

private:
	template<class Archive> friend void boost::serialization::serialize(Archive&, vize::Color&, const unsigned int);
	template<class Archive> friend void boost::serialization::load_construct_data(Archive&, vize::Color*, const unsigned int);
};

}

#endif // VIZE_COLOR_HPP