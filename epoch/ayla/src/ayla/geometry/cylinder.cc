#include "ayla/geometry/cylinder.hpp"

namespace ayla {

bool Cylinder::contains(const glm::vec3& point) const {
	if (glm::dot(_axis.getPQ(), point - _axis.getP()) < 0.0f) {
		return false;
	}

	if (glm::dot(_axis.getQP(), point - _axis.getQ()) < 0.0f) {
		return false;
	}

	return _axis.squaredDistanceTo(point) <= _radius * _radius;
}

}