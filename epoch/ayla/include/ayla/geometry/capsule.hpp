#ifndef AYLA_CAPSULE_HPP
#define AYLA_CAPSULE_HPP

#include "ayla/config.hpp"
#include "ayla/geometry/line_segment.hpp"

namespace ayla {

/**
 * @author O Programador
 */
class AYLA_API Capsule final {
public:
	inline Capsule(const LineSegment& axis, const Float& radius)
		: _axis(axis), _radius(radius)
	{ }

	inline Capsule()
		: _radius(0.0f)
	{ }
	
	inline const LineSegment& getAxis() const {
		return _axis;
	}
	
	inline const Float& getRadius() const {
		return _radius;
	}

	bool collideWithPoint(const glm::vec3& point, glm::vec3& normal, Float& penetration) const;

	inline bool contains(const glm::vec3& point) const {
		return _axis.squaredDistanceTo(point) <= _radius * _radius;
	}
	
private:
	LineSegment _axis;
	Float _radius = 0.0f;
};

}

#endif // AYLA_CAPSULE_HPP