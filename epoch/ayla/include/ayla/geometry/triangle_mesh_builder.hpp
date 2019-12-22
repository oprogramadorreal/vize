#ifndef AYLA_TRIANGLE_MESH_BUILDER_HPP
#define AYLA_TRIANGLE_MESH_BUILDER_HPP

#include "ayla/config.hpp"
#include "ayla/geometry/triangle_mesh.hpp"

namespace ayla {

/**
 * Useful methods to build triangle meshes for basic shapes.
 *
 * @see TriangleMesh
 *
 * @author O Programador
 */
class AYLA_API TriangleMeshBuilder final {
public:
	static std::unique_ptr<TriangleMesh> buildUVSphere(SizeType rings, SizeType sectors, Float radius = 1.0f);

	static std::unique_ptr<TriangleMesh> buildCube(Float sideLength = 1.0f);
};

}

#endif // AYLA_TRIANGLE_MESH_BUILDER_HPP