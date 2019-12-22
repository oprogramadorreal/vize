#include "ayla/geometry/capsule.hpp"

namespace ayla {

bool Capsule::collideWithPoint(const glm::vec3& point, glm::vec3& normal, Float& penetration) const {
	glm::vec3 closestPoint = _axis.getClosestPoint(point);
	glm::vec3 n = point - closestPoint;

	Float distance = glm::length2(n);

	if (distance > _radius*_radius)
		return false;

	if (isZero(distance))
		return false;

	distance = std::sqrt(distance);

	normal = n / distance;
	penetration = _radius - distance;

	return true;
}

}