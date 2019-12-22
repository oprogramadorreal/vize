#include "ayla/geometry/sphere.hpp"
#include <glm/gtx/norm.hpp>

namespace ayla {

Sphere::Sphere(const glm::vec3& minPt, const glm::vec3& maxPt)
	: _c((minPt+maxPt)*0.5f), _r(glm::distance(minPt, _c))
{ }

Sphere::Sphere(const Sphere& a, const Sphere& b) {
	glm::vec3 centreOffset = b._c - a._c;
	Float distance = glm::length2(centreOffset);
	Float radiusDiff = b._r - a._r;

	// Check if the larger sphere encloses the small one
	if (radiusDiff*radiusDiff >= distance) {
		if (a._r > b._r) {
			_c = a._c;
			_r = a._r;

		} else {
			_c = b._c;
			_r = b._r;
		}

	} else { // Otherwise we need to work with partially overlapping spheres
		distance = glm::sqrt(distance);
		_r = (distance + a._r + b._r) * Float(0.5f);

		// The new centre is based on one's centre, moved towards
		// two's centre by an ammount proportional to the spheres' radii.
		_c = a._c;

		if (distance > 0) {
			_c += centreOffset * ((_r - a._r)/distance);
		}
	}
}

bool Sphere::contains(const glm::vec3& pt) const {
	return ( glm::length2(pt-_c) <= (_r*_r) );
}

void Sphere::translate(const glm::vec3& t) {
	_c += t;
}

Sphere Sphere::translated(const glm::vec3& t) const {
	Sphere cpy(*this);
	cpy.translate(t);
	return cpy;
}

bool Sphere::intersects(const Sphere& other) const {
	// Find the vector between the objects
	const glm::vec3 midline = _c - other._c;

	// See if it is large enough.
	return glm::length2(midline) < ( _r+other._r ) * ( _r+other._r );
}

void Sphere::getDirExtremePts(const glm::vec3& normDir, glm::vec3& posDirPt, glm::vec3& negDirPt) const {
	const glm::vec3 normDirTimesRadius(normDir * _r);
	posDirPt = (_c + normDirTimesRadius);
	negDirPt = (_c - normDirTimesRadius);
}

Float Sphere::getVolume() const {
	return Float(4.0/3.0) * glm::pi<Float>() * (_r*_r*_r);
}

bool Sphere::collideWithPoint(const glm::vec3& point, glm::vec3& normal, Float& penetration) const {
	const glm::vec3 n = point - getCenter();
	const Float nSqrMag = glm::length2(n);

	if (nSqrMag > (getRadius()*getRadius())) {
		return false;
	}

	if (isZero(nSqrMag)) {
		return false; // we cannot calculate a normal vector for this case!
	}

	const Float nMag = std::sqrt(nSqrMag);

	normal = n / nMag;
	penetration = getRadius() - nMag;

	return true;
}

}
