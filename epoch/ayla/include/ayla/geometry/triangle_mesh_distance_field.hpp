#ifndef AYLA_TRIANGLE_MESH_DISTANCE_FIELD_HPP
#define AYLA_TRIANGLE_MESH_DISTANCE_FIELD_HPP

#include "ayla/config.hpp"

#include "ayla/geometry/triangle_mesh.hpp"
#include "ayla/geometry/voxel_grid.hpp"
#include "ayla/serialization/triangle_mesh_distance_field_serializer.hpp"

#include <boost/noncopyable.hpp>

namespace ayla {

class RegularGridIndexer;

/**
 * For a given triangle mesh, implements the abstraction called "distance field" (aka "distance volume" or "distance function").
 * It provides very fast triangle mesh distance queries for 3D points. It returns negative distances if the point is inside the mesh volume.
 *
 * @see Teschner, M., & Kimmerle, S. (2005). Collision detection for deformable objects.
 * Retrieved from http://onlinelibrary.wiley.com/doi/10.1111/j.1467-8659.2005.00829.x/full
 *
 * @see ayla::TriangleMeshDistanceFieldBuilder
 * 
 * @author O Programador
 */
class AYLA_API TriangleMeshDistanceField final : boost::noncopyable {
public:
	TriangleMeshDistanceField();
	~TriangleMeshDistanceField();

	inline const TriangleMesh& getTriangleMesh() const { return *_mesh; }

	AxisAlignedBox getAABB() const;

	/**
	 * @return The distance of a point to the represented triangle mesh. Distance are negative for internal points.
	 * If the point is considerably far then infinity is returned.
	 */
	Float distance(const glm::vec3& point) const;

	/**
	 * The same as the other 'distance' method but here if the returned distance is less than 'collisionEpsilon' then 'surfaceNormal' is also calculated.
	 */
	Float distance(const glm::vec3& point, glm::vec3& surfaceNormal, Float collisionEpsilon = 0.0f) const;

private:
	std::unique_ptr<const TriangleMesh> _mesh;
	FloatVoxelGrid _voxelGrid;

	friend class TriangleMeshDistanceFieldBuilder;
	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::TriangleMeshDistanceField&, const unsigned int);
};

}

#endif // AYLA_TRIANGLE_MESH_DISTANCE_FIELD_HPP
