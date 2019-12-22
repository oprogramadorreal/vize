#include "ayla/geometry/plane.hpp"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/epsilon.hpp>

namespace ayla {

Plane::Plane(const glm::vec3& normal, Float distance)
	: _n(normal), _d(distance)
{
	assert(glm::epsilonEqual(glm::length2(normal), 1.0f, AYLA_PLANE_DEFAULT_EPSILON));
}

void Plane::set(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC) {
	_n = glm::cross(ptB - ptA, ptC - ptB);
	_n = glm::normalize(_n);
	_d = glm::dot(_n, ptA);
}

void Plane::set(const glm::vec3& normal, const glm::vec3& pt) {
	assert(glm::epsilonEqual(glm::length2(normal), 1.0f, AYLA_PLANE_DEFAULT_EPSILON));

	_n = normal;
	_d = glm::dot(normal, pt);
}

bool Plane::belongs(const glm::vec3& pt, Float tolerance) const {
	return isZero(getDistance(pt), tolerance);
}

glm::vec3 Plane::projectVector(const glm::vec3& v) const {
	const Float dist = glm::dot(v, _n);
	return v - dist * _n;
}

glm::vec3 Plane::projectPoint(const glm::vec3& point) const {
	const glm::vec3 pt = point - getOrigin();
	const Float dist = glm::dot(pt, _n);
	return point - dist * _n;
}

// Real-time collision detection pg. 210
bool Plane::intersection(const Plane& other, glm::vec3& p, glm::vec3& d, Float epsilon) const {
	// Compute direction of intersection line
	d = glm::cross(_n, other._n);

	// If d is (near) zero, the planes are parallel (and separated)
	// or coincident, so they’re not considered intersecting
	Float denom = glm::dot(d, d);
	
	if (denom < epsilon) {
		return false;
	}

	// Compute point on intersection line
	p = glm::cross(other._n*_d - _n*other._d, d) * (1.0f/denom);
	
	return true;
}

// Real-Time Collision Detection pg. 176
bool Plane::intersection(const glm::vec3& a, const glm::vec3& b, glm::vec3& intrsPt) const {
	// Compute the t value for the directed line ab intersecting the plane
	const glm::vec3 ab = b - a;
	const Float t = -getDistance(a) / glm::dot(getNormal(), ab);

	// If t in [0..1] compute and return intersection point
	if (t >= 0.0f && t <= 1.0f) {
		intrsPt = a + ab*t;
		return true;
	}

	return false; // no intersection
}

Plane::Side Plane::getSide(const glm::vec3& pt, Float epsilon) const {
	const Float dist = getDistance(pt);

	if ( dist < -epsilon ) {
		return Plane::PS_NEGATIVE;
	}

	if ( dist > epsilon ) {
		return Plane::PS_POSITIVE;
	}

	return Plane::PS_BELONGS;
}

glm::vec3 Plane::getMirroredPoint(const glm::vec3& pt) const {
	return pt + getNormal()*(-2.0f*getDistance(pt));
}

}
