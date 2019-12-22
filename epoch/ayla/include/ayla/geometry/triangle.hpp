#ifndef AYLA_TRIANGLE_HPP
#define AYLA_TRIANGLE_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/plane.hpp"
#include "ayla/geometry/sphere.hpp"
#include "ayla/geometry/axis_aligned_box.hpp"

namespace ayla {

/**
 * A triangle in a three-dimensional space.
 * 
 * @author O Programador
 */
class AYLA_API Triangle final {
public:	
	Triangle() = default;

	inline Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : _a(a), _b(b), _c(c) { }
	inline Triangle(const Triangle& other) : _a(other._a), _b(other._b), _c(other._c) { }
	inline ~Triangle() { }

	inline const glm::vec3& getA() const { return _a; }
	inline const glm::vec3& getB() const { return _b; }
	inline const glm::vec3& getC() const { return _c; }

	/**
	 * @return A point on the triangle that has the given barycentric coordinates.
	 * @see getBarycentricCoordinates
	 */
	inline glm::vec3 getWithBarycentric(Float u, Float v, Float w) const {
		return u*_a + v*_b + w*_c;
	}

	inline const glm::vec3& operator[](SizeType i) const {
		assert(i < 3u);
		return *((&_a)+i);
	}

	glm::vec3 getCenter() const;

	inline Plane getPlane() const { return Plane(_a, _b, _c); }

	/**
	 * @return A vector that is orthogonal to the plane of this triangle.
	 * @warning The returned vector does not have unit length (i.e., it is not normalized).
	 */
	inline glm::vec3 getOrthogonalVector() const { return glm::cross(_b - _a, _c - _a); }

	/**
	 * Computes the triangle's normal vector. The normal is defined as (b - a) x (c - a)
	 */
	inline glm::vec3 getNormal() const { return glm::normalize(getOrthogonalVector()); }

	Float calculateArea() const;

	Float calculatePerimeter() const;

	/**
	 * Calculate the point on the triangle that is the closest to @param point.
	 * It also calculates the barycentric coordinates (u,v,w) of the returned point.
	 */
	glm::vec3 getClosestPoint(const glm::vec3& point, Float& u, Float& v, Float& w) const;

	inline glm::vec3 getClosestPoint(const glm::vec3& point) const {
		Float u, v, w; // ignore barycentric coordinates
		return getClosestPoint(point, u, v, w);
	}

	/**
	 * Test if @param point belongs to this triangle region.
	 */
	bool belongs(const glm::vec3& point) const;

	/**
	 * Test if @param point belongs to this triangle region with a given @param epsilon tolerance.
	 */
	bool epsilonBelongs(const glm::vec3& point, Float epsilon) const;

	/**
	 * Performs an intersection test between this and 'other' Triangle.
	 *
	 * @return True if the two triangles intersects.
	 * @remarks To perform intersection test between different primitives use the IntersectionTests interface.
	 * @see IntersectionTests
	 *
	 * @TOFIX this does not work for parallel triangles.
	 */
	bool intersects(const Triangle& other) const;

	/**
	 * Returns whether segment pq intersects this triangle.
	 */
	bool intersects(const glm::vec3& p, const glm::vec3& q) const;

	void calculateAABB(glm::vec3& minPt, glm::vec3& maxPt) const;

	inline AxisAlignedBox calculateAABB() const {
		glm::vec3 minPt, maxPt;
		calculateAABB(minPt, maxPt);
		return AxisAlignedBox(minPt, maxPt);
	}

	Sphere calculateBoundingSphere() const;

	/** 
	 * Compute barycentric coordinates (u, v, w) for point @param p with respect to this triangle.
	 */
	void getBarycentricCoordinates(const glm::vec3& p, Float &u, Float &v, Float &w) const;

private:
	glm::vec3 _a = glm::vec3(0.0f);
	glm::vec3 _b = glm::vec3(0.0f);
	glm::vec3 _c = glm::vec3(0.0f);
};

}

#endif // AYLA_TRIANGLE_HPP
