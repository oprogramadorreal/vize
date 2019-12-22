#include "vize/model/transfer_function/control_point.hpp"

namespace vize {

ControlPoint::ControlPoint() = default;

ControlPoint::ControlPoint(Float position)
	: ControlPoint(position, Color())
{ }

ControlPoint::ControlPoint(Float position, Color color) {
	setPosition(position);
	setColor(color);
}

void ControlPoint::setPosition(Float newPosition) {
	_position = Float(glm::clamp(newPosition, 0.0f, 1.0f));
}

void ControlPoint::setColor(const Color& newColor) {
	_color = newColor;
}

Float ControlPoint::getPosition() const {
	return _position;
}

Color ControlPoint::getColor() const {
	return _color;
}

glm::vec2 ControlPoint::getNormalized2DPosition() const {
	return glm::vec2(_position, getColor().getNormalizedA());
}

void ControlPoint::setNormalized2DPosition(const glm::vec2& normalized2dPosition) {
	setPosition(normalized2dPosition.x);

	const auto currentColor = getColor();
	const auto clampedAlpha = Float(glm::clamp(normalized2dPosition.y, 0.0f, 1.0f));

	setColor(
		Color(
			currentColor.getR(),
			currentColor.getG(),
			currentColor.getB(),
			Color::ChannelType(clampedAlpha * Float(std::numeric_limits<Color::ChannelType>::max()))
		)
	);
}

}