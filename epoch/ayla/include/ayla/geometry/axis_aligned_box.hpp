#ifndef AYLA_AXIS_ALIGNED_BOX_HPP
#define AYLA_AXIS_ALIGNED_BOX_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/line_segment.hpp"
#include "ayla/serialization/axis_aligned_box_serializer.hpp"

#include <vector>
#include <array>

namespace ayla {

/**
 * Represents an axis-aligned box in a three-dimensional space.
 *
 *                (2)________________(6)
 *                  |\               |\
 *                  | \              | \
 *                  |  \             |  \
 *                  |(3)\________________\(7) = max vertex  
 *                  |   |            |   |
 *  min vertex = (0)|_ _|_ _ _ _ _ _ |(4)|
 *                   \  |             \  |
 *                    \ |              \ |
 *                     \|_______________\|
 *                     (1)               (5)
 *     |y+
 *     |
 *     |_____x+
 *      \
 *       \z+
 *
 * @see Box
 * 
 * @author O Programador
 */
class AYLA_API AxisAlignedBox final {
public:
	AxisAlignedBox();

	AxisAlignedBox(const AxisAlignedBox& other);

	/**
	 * Construct an AABB enclosing all the points in the @param setOfPoints
	 */
	explicit AxisAlignedBox(const std::vector<glm::vec3>& setOfPoints);
	explicit AxisAlignedBox(const std::vector<glm::vec2>& setOfPoints);

	AxisAlignedBox(const glm::vec3& center, const Float& halfWidth, const Float& halfHeight, const Float& halfDepth);
	AxisAlignedBox(const glm::vec3& minPt, const glm::vec3& maxPt);

	/**
	 * Creates an AABB enclosing two other AABBs.
	 */
	AxisAlignedBox(const AxisAlignedBox& one, const AxisAlignedBox& two);

	/**
	 * @param idx Integer value in the range [0, 7].
	 * @see AxisAlignedBox description.
	 */
	glm::vec3 getVertex(SizeType index) const;

	/**
	 * @return All the 8 vertices of this AABB.
	 */
	std::array<glm::vec3, 8> getVertices() const;

	/**
	 * @return All the 12 edgs of this AABB.
	 */
	std::array<LineSegment, 12> getEdges() const;

	void translate(const glm::vec3& t);

	/**
	 * @return A copy of this AABB translated by the provided vector in @param t.
	 */
	AxisAlignedBox translated(const glm::vec3& t) const;

	/**
	 * The rotations of the transform will modify this AABB. The resultant AABB will be equivalent to
	 * the one you get if you create a new AABB that encloses the box resultant of the rotation of this.
	 */
	void transform(const glm::vec3& translation, const glm::mat3& rotation);

	/**
	 * Transform this AABB by a full transformation matrix @param modelMatrix.
	 */
	bool transform(const glm::mat4& modelMatrix);

	/**
	 * @return A copy of this AABB transformed by the provided RigidTransform in @param t.
	 */
	AxisAlignedBox transformed(const glm::vec3& translation, const glm::mat3& rotation) const;

	glm::vec3 getCenter() const;
	void setCenter(const glm::vec3& center);

	Float getWidth() const;
	Float getHeight() const;
	Float getDepth() const;

	Float getHalfWidth() const;
	Float getHalfHeight() const;
	Float getHalfDepth() const;

	/**
	 * @return A glm::vec3 containing the half of the width, height and depth of this AABB in x, y and z coordinates, respectively.
	 */
	glm::vec3 getHalfExtensions() const;

	/**
	 * @return A glm::vec3 containing the width, height and depth of this AABB in x, y and z coordinates, respectively.
	 */
	glm::vec3 getExtensions() const;

	/**
	 * Returns a point in which the values of x,y and z coordinates are bigger than any other point inside this AABB.
	 */
	glm::vec3 getMaxPoint() const;

	/**
	 * Returns a point in which the values of x,y and z coordinates are smaller than any other point inside this AABB.
	 */
	glm::vec3 getMinPoint() const;

	void getAABBLimits(glm::vec3& minPt, glm::vec3& maxPt) const;

	/**
	 * Calculates the volume of this AABB.
	 */
	Float getVolume() const;

	/**
	 * Given point pt, return the point on or in this AxisAlignedBox that is closest to p
	 */
	glm::vec3 getClosestPoint(const glm::vec3& pt) const;

	/** 
	 * Computes the square distance between a point pt and this AxisAlignedBox
	 */
	Float getSqrDistance(const glm::vec3 &pt) const;

	/**
	 * Returns the farthest vertices of this AABB projected onto a given direction.
	 *
	 * @warning 'normDir' must be an unit vector.
	 */
	void getDirExtremePts(const glm::vec3& normDir, glm::vec3& posDirPt, glm::vec3& negDirPt) const;

	/**
	 * Performs an intersection test between this and 'other' AABB.
	 *
	 * @return True if the two AABBs intersects.
	 * @remarks To perform intersection test between different primitives use the IntersectionTests interface.
	 * @see IntersectionTests
	 */
	bool intersects(const AxisAlignedBox& other) const;

	/**
	 * @return True if this aabb intersects point 'pt'.
	 */
	bool contains(const glm::vec3& pt) const;

	/**
	 * @return True if this AxisAlignedBox contains other.
	 */
	bool contains(const AxisAlignedBox& other) const;

	/**
	 * Enlarge the dimensions of this AABB by twice the values provided in each coordinate of the glm::vec3 in @param e.
	 */
	void expand(const glm::vec3& e);

	/**
	 * Enlarge the dimensions of this AABB by the values provided in each coordinate of the glm::vec3 in @param e.
	 * Negative values will be added to the min point and positive values to the max point of this AABB.
	 */
	void signedExpand(const glm::vec3& e);

	/**
	 * @return True if this AABB is different from @param other
	 */
	bool notEqual(const AxisAlignedBox& other) const;

	/**
	 * Clip @param segment against an AABB.
	 * 
	 * @return TRUE on success. @param clipedSegment will have a valid segment bounded by the AABB.
	 * If @param segment is completely outside the AABB, then this will return false.
	 *
	 * @TODO test it
	 */
	bool clipLineSegment(const LineSegment& segment, LineSegment& clipedSegment) const;

	/**
	 * @return If this AxisAlignedCox contains the given point, it returns a point where each component is in the range [0,1].
	 * The closer the point is to minPoint, closer the result is to (0,0,0). The closer the point is to maxPoint, closer the
	 * result is to (1,1,1).
	 */
	glm::vec3 relativePosition(const glm::vec3& point) const;

public:
	/**
	 * Calculates an AxisAlignedBox that encapsulates the two others.
	 */
	static void merge(const AxisAlignedBox& a, const AxisAlignedBox& b, AxisAlignedBox& result);

	static AxisAlignedBox merge(const AxisAlignedBox& a, const AxisAlignedBox& b);

private:
	glm::vec3 _minPt = glm::vec3(0.0f);
	glm::vec3 _maxPt = glm::vec3(0.0f);

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::AxisAlignedBox&, const unsigned int);
};

}

#endif // AYLA_AXIS_ALIGNED_BOX_HPP
