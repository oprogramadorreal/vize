#ifndef AYLA_TRIANGLE_MESH_DISTANCE_FIELD_BUILDER_HPP
#define AYLA_TRIANGLE_MESH_DISTANCE_FIELD_BUILDER_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/triangle_mesh_distance_field.hpp"

namespace ayla {

/**
 * Knows how to create a triangle mesh distance field.
 *
 * @see ayla::TriangleMeshDistanceField
 * 
 * @author O Programador
 */
class AYLA_API TriangleMeshDistanceFieldBuilder final {
public:
	/**
	 * @warning Very inefficient. Please, consider saving the instance of this object after building.
	 */
	std::unique_ptr<TriangleMeshDistanceField> build(std::unique_ptr<const TriangleMesh> mesh, const glm::vec3& cellsSize, const glm::vec3& aabbVolumeExpansion = glm::vec3(0.5f)) const;

private:
	std::unique_ptr<TriangleMeshDistanceField> _create(std::unique_ptr<const TriangleMesh> mesh, const glm::vec3& cellsSize, const glm::vec3& aabbVolumeExpansion) const;
	void _calculateDistances(TriangleMeshDistanceField& meshDistanceField) const;
};

}

#endif // AYLA_TRIANGLE_MESH_DISTANCE_FIELD_BUILDER_HPP
