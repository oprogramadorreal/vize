#ifndef VIZE_CONTROL_POINT_HPP
#define VIZE_CONTROL_POINT_HPP

#include "vize/config.hpp"

#include "vize/model/color.hpp"
#include "vize/serialization/control_point_serializer.hpp"

namespace vize {

/**
 * A control point in a transfer function.
 *
 * @see vize::TransferFunction
 *
 * @author O Programador
 */
class ControlPoint final {
public:
	ControlPoint();
	explicit ControlPoint(Float position);
	ControlPoint(Float position, Color color);

public:
	void setPosition(Float newPosition);
	void setColor(const Color& newColor);

	Float getPosition() const;
	Color getColor() const;

	glm::vec2 getNormalized2DPosition() const;
	void setNormalized2DPosition(const glm::vec2& normalized2dPosition);
	
private:
	Float _position = Float(0.0f);
	Color _color;

private:
	template<class Archive> friend void boost::serialization::serialize(Archive&, vize::ControlPoint&, const unsigned int);
};

}

#endif // VIZE_CONTROL_POINT_HPP