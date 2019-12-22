#ifndef AYLA_SPHERE_HPP
#define AYLA_SPHERE_HPP

#include "ayla/config.hpp"
#include "ayla/geometry/axis_aligned_box.hpp"

namespace ayla {

/**
 * A basic class to represent a sphere.
 * 
 * @author O Programador
 */
class AYLA_API Sphere final {
public:
	/**
	 * Sphere at position zero with radius zero
	 */
	Sphere() = default;

	inline Sphere(const glm::vec3& center, const Float& radius) : _c(center), _r(radius) { }
 
	/**
	 * Create a sphere that encloses an AABB.
	 */
	Sphere(const glm::vec3& minPt, const glm::vec3& maxPt);

	/**
	 * Creates a sphere enclosing two other spheres.
	 */
	Sphere(const Sphere& a, const Sphere& b);

	inline glm::vec3 getCenter() const { return _c; }
	inline Float getRadius() const { return _r; }

	inline void setCenter(const glm::vec3& center) { _c = center; }
	inline void setRadius(const Float& radius) { _r = radius; }

	bool contains(const glm::vec3& pt) const;

	void translate(const glm::vec3& t);

	Sphere translated(const glm::vec3& t) const;

	/**
	 * Returns the most extreme points for a given direction.
	 *
	 * @warning 'normDir' must be an unit vector.
	 */
	void getDirExtremePts(const glm::vec3& normDir, glm::vec3& posDirPt, glm::vec3& negDirPt) const;
 
	/**
	 *  calculate an axis-aligned bounding box (AABB) containing this sphere 
	 */
	inline AxisAlignedBox getAABB() const {
		return AxisAlignedBox(
			glm::vec3(_c.x - _r, _c.y - _r, _c.z - _r),
			glm::vec3(_c.x + _r, _c.y + _r, _c.z + _r)
		);
	}

	/**
	 * Performs an intersection test between this and 'other' Sphere.
	 *
	 * @return True if the two spheres intersects.
	 * @remarks To perform intersection test between different primitives use the IntersectionTests interface.
	 * @see IntersectionTests
	 */
	bool intersects(const Sphere& other) const;

	Float getVolume() const;

	bool collideWithPoint(const glm::vec3& point, glm::vec3& normal, Float& penetration) const;

private:
	glm::vec3 _c = glm::vec3(0.0f);
	Float _r = 0.0f;
};

}

#endif // AYLA_SPHERE_HPP
