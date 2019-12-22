#ifndef AYLA_TRIANGLE_MESH_SERIALIZER_HPP
#define AYLA_TRIANGLE_MESH_SERIALIZER_HPP

namespace ayla {
	class TriangleMesh;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::TriangleMesh& mesh, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, ayla::TriangleMesh* mesh, const unsigned int version);

}
}

#include "ayla/serialization/serializer.hpp"

namespace ayla {

/**
 * Knows how to serialize a triangle mesh.
 *
 * @author O Programador
 */
class AYLA_API TriangleMeshSerializer final : public Serializer<TriangleMesh> {
public:
	TriangleMeshSerializer();
};

}

#endif // AYLA_TRIANGLE_MESH_SERIALIZER_HPP