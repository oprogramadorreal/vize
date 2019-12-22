#include "ayla/geometry/ray.hpp"
#include "ayla/geometry/vector.hpp"

namespace ayla {

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
	: _origin(origin), _direction(direction)
{
	assert(glm::epsilonEqual(glm::length(_direction), 1.0f, 0.000001f));
}

Ray::Ray(const Ray& other)
	: _origin(other._origin), _direction(other._direction)
{ }

glm::vec3 Ray::getPointAt(const Float &distance) const {
	assert(distance >= 0.0f);
	assert(glm::epsilonEqual(glm::length(_direction), 1.0f, 0.000001f));

	return _origin + (_direction * distance);
}

Float Ray::distanceTo(const glm::vec3& pt, const Float& maxDist) const {
	// http://www.softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm

	const glm::vec3 end = getPointAt(maxDist);

	const glm::vec3 v = end - _origin;
	const glm::vec3 w = pt - _origin;
	const Float c1 = glm::dot(w, v);
	const Float c2 = glm::dot(v, v);

	if (c1 <= 0.0f) {
		return glm::distance(pt, _origin);
	}

	if (c2 <= c1) {
		return glm::distance(pt, end);
	}

	const Float b = c1/c2;
	const glm::vec3 pb = _origin + b * v;

	return glm::distance(pt, pb);
}

std::ostream& operator<<(std::ostream &out, const Ray& ray) {
	out << "Ray(" << ray._origin << ", " << ray._direction << ")";
	return out;
}

}
