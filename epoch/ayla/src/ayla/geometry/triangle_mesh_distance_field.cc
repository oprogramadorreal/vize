#include "ayla/geometry/triangle_mesh_distance_field.hpp"

#include "ayla/geometry/regular_grid_indexer.hpp"

namespace ayla {

TriangleMeshDistanceField::TriangleMeshDistanceField() = default;

TriangleMeshDistanceField::~TriangleMeshDistanceField() = default;

AxisAlignedBox TriangleMeshDistanceField::getAABB() const {
	return _voxelGrid.getAABB();
}

Float TriangleMeshDistanceField::distance(const glm::vec3& point) const {
	if (!_voxelGrid.contains(point)) {
		return std::numeric_limits<Float>::infinity();
	}

	return _voxelGrid.sample(point);
}

Float TriangleMeshDistanceField::distance(const glm::vec3& point, glm::vec3& surfaceNormal, Float collisionEpsilon) const {
	const auto distanceValue = distance(point);

	if (distanceValue < collisionEpsilon) {
		const auto gradient = _voxelGrid.sampleGradient(point);

		if (!isZero(glm::length2(gradient))) {
			surfaceNormal = glm::normalize(gradient);
		}
	}

	return distanceValue;
}

}
