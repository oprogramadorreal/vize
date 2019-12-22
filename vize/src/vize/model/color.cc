#include "vize/model/color.hpp"

namespace vize {

Color::Color() 
	: Color(ChannelType(0), ChannelType(0), ChannelType(0))
{ }

Color::Color(ChannelType r, ChannelType g, ChannelType b, ChannelType a)
	: _r(r), _g(g), _b(b), _a(a)
{ }

Color::ChannelType Color::getR() const { return _r; }
Color::ChannelType Color::getG() const { return _g; }
Color::ChannelType Color::getB() const { return _b; }
Color::ChannelType Color::getA() const { return _a; }

Float Color::getNormalizedR() const { return getR() / Float(std::numeric_limits<ChannelType>::max()); }
Float Color::getNormalizedG() const { return getG() / Float(std::numeric_limits<ChannelType>::max()); }
Float Color::getNormalizedB() const { return getB() / Float(std::numeric_limits<ChannelType>::max()); }
Float Color::getNormalizedA() const { return getA() / Float(std::numeric_limits<ChannelType>::max()); }

Color Color::operator+(const Color& other) const {
	return Color(
		_r + other._r,
		_g + other._g,
		_b + other._b,
		_a + other._a
	);
}

Color Color::operator*(Float value) const {
	return Color(
		ChannelType(Float(_r) * value),
		ChannelType(Float(_g) * value),
		ChannelType(Float(_b) * value),
		ChannelType(Float(_a) * value)
	);
}

Color Color::white() { return Color(ChannelType(255), ChannelType(255), ChannelType(255)); }
Color Color::black() { return Color(ChannelType(0), ChannelType(0), ChannelType(0)); }
Color Color::red() { return Color(ChannelType(255), ChannelType(0), ChannelType(0)); }
Color Color::green() { return Color(ChannelType(0), ChannelType(255), ChannelType(0)); }
Color Color::blue() { return Color(ChannelType(0), ChannelType(0), ChannelType(255)); }

}