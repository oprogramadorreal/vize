#ifndef AYLA_PLANE_HPP
#define AYLA_PLANE_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/plane_serializer.hpp"

#define AYLA_PLANE_DEFAULT_EPSILON 0.00001f

namespace ayla {

/** 
 * An abstraction to represent a plane in a three-dimensional space.
 *
 * @author O Programador
 */ 
class AYLA_API Plane final {
public:
	enum Side {
		PS_NEGATIVE = 0,
		PS_POSITIVE,
		PS_BELONGS,
		PS_BOTH
	};

public:
	inline static Plane getXY() { return Plane(glm::vec3(0.0f, 0.0f, 1.0f), 0.0f); }
	inline static Plane getXZ() { return Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f); }
	inline static Plane getYZ() { return Plane(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f); }

public:
	/**
	 * @warning This creates an invalid plane (normal has zero length).
	 */
	inline Plane()
		: _n(0.0f, 0.0f, 0.0f), _d(0.0f)
	{ }

	Plane(const glm::vec3& normal, Float distance);

	/**
	 * Constructs a plane using the Hessian Form
	 * A point (x,y,z) is in the plane if and only if a*x + b*y + c*z + d = 0
	 */
	inline Plane(Float a, Float b, Float c, Float d)
		: _n(a, b, c), _d(-d)
	{ }

	inline Plane(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC) { set(ptA, ptB, ptC); }

	inline Plane(const glm::vec3& normal, const glm::vec3& pt) { set(normal, pt); }

	void set(const glm::vec3& ptA, const glm::vec3& ptB, const glm::vec3& ptC);

	void set(const glm::vec3& normal, const glm::vec3& pt);

	inline void normalize() { _n = glm::normalize(_n); }

	inline Float getDistance(const glm::vec3& pt) const { return glm::dot(_n, pt) - _d; }

	/**
	 * Test if point 'pt' belongs to this plane.
	 */
	bool belongs(const glm::vec3& pt, Float tolerance = AYLA_PLANE_DEFAULT_EPSILON) const;

	/**
	 * Given planes p1 and p2, compute line L = p+t*d of their intersection.
	 *
	 * @return 'false' if planes do not intersects.
	 */
	bool intersection(const Plane& other, glm::vec3& p, glm::vec3& d, Float epsilon = AYLA_PLANE_DEFAULT_EPSILON) const;

	/**
	 * Given segment ab, computes the intersection point with this plane.
	 */
	bool intersection(const glm::vec3& a, const glm::vec3& b, glm::vec3& intrsPt) const;

	/** 
	 * Project a vector onto the plane. 
	 *
	 * @remarks This gives you the element of the input vector that is perpendicular 
	 * to the normal of the plane. You can get the element which is parallel
	 * to the normal of the plane by subtracting the result of this method
	 * from the original vector, since parallel + perpendicular = original.
	 */
	glm::vec3 projectVector(const glm::vec3& v) const;
	
	/**
	 * Projects the point onto the plane.
	 * 
	 * @return the closest point on the plane to the given point.
	 */
	glm::vec3 projectPoint(const glm::vec3& pt) const;

	inline const glm::vec3& getNormal() const {
		return _n;
	}

	/**
	 * @return A point onto the plane.
	 */
	inline glm::vec3 getOrigin() const {
		return _n*_d;
	}

	Side getSide(const glm::vec3& pt, Float epsilon = AYLA_PLANE_DEFAULT_EPSILON) const;

	inline Float getD() const { return _d; }

	/**
	 * @return A point that is point @param pt reflected to the other side of this plane.
	 */
	glm::vec3 getMirroredPoint(const glm::vec3& pt) const;

	/**
	 * @return A component from the Plane using the Hessian Form
	 */
	inline Float hessianA() const { return _n.x; }

	/**
	 * @return B component from the Plane using the Hessian Form
	 */
	inline Float hessianB() const { return _n.y; }

	/**
	 * @return C component from the Plane using the Hessian Form
	 */
	inline Float hessianC() const { return _n.z; }

	/**
	 * @return D component from the Plane using the Hessian Form
	 */
	inline Float hessianD() const { return -_d; }

private:
	glm::vec3 _n; // Plane normal. Any points x on the plane satisfy Dot(n,x) = d
	Float _d = 0.0f; // d = dot(n,x) for a given point x on the plane

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::Plane&, const unsigned int);
};

}

#endif // AYLA_PLANE_HPP
