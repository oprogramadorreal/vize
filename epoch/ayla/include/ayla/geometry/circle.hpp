#ifndef AYLA_CIRCLE_HPP
#define AYLA_CIRCLE_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/plane.hpp"

#include <glm/gtc/epsilon.hpp>

namespace ayla {

/**
 * A circle in 3D space.
 *
 * @author O Programador
 */
class AYLA_API Circle final {
public:
	inline Circle(Float radius, const glm::vec3& center, const glm::vec3& axis)
		: _radius(radius), _center(center), _axis(axis)
	{
		assert(glm::epsilonEqual(glm::length(_axis), 1.0f, 0.000001f));
	}

	inline Circle()
		: _radius(0.0f), _center(0, 0, 0), _axis(0, 1, 0)
	{ }

	inline Float getRadius() const { return _radius; }
	inline const glm::vec3& getCenter() const { return _center; }
	inline const glm::vec3& getAxis() const { return _axis; }

	inline void setCenter(const glm::vec3& center) { _center = center; }
	inline void setRadius(Float radius) { _radius = radius; }

	inline Plane getPlane() const { return Plane(_axis, _center); }

	/**
	 * @param Angle in radians ranging from 0 to 2*PI.
	 */
	glm::vec3 getPointOnCircumference(Float angle) const;
		
private:
	Float _radius;
	glm::vec3 _center;
	glm::vec3 _axis;
};

}

#endif // AYLA_CIRCLE_HPP