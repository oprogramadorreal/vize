#ifndef AYLA_BOX_HPP
#define AYLA_BOX_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/axis_aligned_box.hpp"
#include "ayla/geometry/rigid_transform.hpp"

#include <glm/gtc/matrix_access.hpp>

namespace ayla {

/**
 * Represents an oriented box in a three-dimensional space.
 * 
 * @see AxisAlignedBox
 * 
 * @author O Programador
 */
class AYLA_API Box final {
public:
	Box();
	Box(const Box& other);
	Box(const AxisAlignedBox& aab);
	Box(const glm::vec3& center, const Float& halfWidth, const Float& halfHeight, const Float& halfDepth, const glm::mat3& base = glm::mat3(1.0f));

	RigidTransform getTransform() const;

	void setTransform(const RigidTransform& t);

	std::array<glm::vec3, 8> getVertices() const;
	std::array<LineSegment, 12> getEdges() const;

	inline glm::vec3 getCenter() const { return _aab.getCenter(); }
	inline void setCenter(const glm::vec3& center) { _aab.setCenter(center); }

	inline Float getWidth() const { return _aab.getWidth(); } 
	inline Float getHeight() const { return _aab.getHeight(); } 
	inline Float getDepth() const { return _aab.getDepth(); } 

	inline Float getHalfWidth() const { return _aab.getHalfWidth(); } 
	inline Float getHalfHeight() const { return _aab.getHalfHeight(); } 
	inline Float getHalfDepth() const { return _aab.getHalfDepth(); }

	inline glm::vec3 getHalfExtensions() const { return _aab.getHalfExtensions(); } 

	inline Float getVolume() const { return _aab.getVolume(); }

	// calculate an axis-aligned bounding box (AABB) containing this box 
	AxisAlignedBox calculateAABB() const;

	inline glm::vec3 getAxis(SizeType i) const {
		assert(i < 3);
		return glm::column(_base, i);
	}

	/**
	 * Returns the most extreme points for a given direction.
	 *
	 * @warning 'normDir' must be an unit vector.
	 */
	void getDirExtremePts(const glm::vec3& normDir, glm::vec3& posDirPt, glm::vec3& negDirPt) const;

	/**
	 * Performs an intersection test between this and 'other' Box.
	 *
	 * @return True if the two boxes intersects.
	 * @remarks To perform intersection test between different primitives use the IntersectionTests interface.
	 * @see IntersectionTests
	 */
	bool intersects(const Box& other) const;

	// Returns whether or not this box intersects point 'pt'.
	// @todo test it
	inline bool contains(const glm::vec3& pt) const {
		return _aab.contains(getTransform().transformInverse(pt));
	}

	inline const glm::mat3& getBase() const {
		return _base;
	}

	inline void rotateBase(const glm::mat3& rot) {
		_base = rot*_base;
	}

	bool collideWithPoint(const glm::vec3& point, glm::vec3& normal, Float& penetration) const;

private:
	AxisAlignedBox _aab = AxisAlignedBox();
	glm::mat3 _base = glm::mat3(1.0f);
};

}

#endif // AYLA_BOX_HPP
