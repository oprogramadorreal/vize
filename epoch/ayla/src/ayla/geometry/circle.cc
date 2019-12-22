#include "ayla/geometry/circle.hpp"
#include "ayla/geometry/vector.hpp"

namespace ayla {

glm::vec3 Circle::getPointOnCircumference(Float angle) const {
	// Based on http://math.stackexchange.com/questions/73237/parametric-equation-of-a-circle-in-3d-space
	const auto a = ayla::orthogonal(_axis);
	const auto b = glm::cross(a, _axis);

	return _center + _radius * glm::cos(angle) * a + _radius * glm::sin(angle) * b;
}

}