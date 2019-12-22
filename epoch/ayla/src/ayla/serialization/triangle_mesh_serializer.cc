#include "ayla/serialization/triangle_mesh_serializer.hpp"
#include "ayla/geometry/triangle_mesh.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::TriangleMesh& mesh, const unsigned int version) {
	ar & make_nvp("vertices", mesh._vertices);
	ar & make_nvp("faces", mesh._faces);
}

template<class Archive>
void load_construct_data(Archive& ar, ayla::TriangleMesh* mesh, const unsigned int version) {
	::new(mesh)ayla::TriangleMesh();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::TriangleMesh, AYLA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(ayla::TriangleMesh, AYLA_API);

}
}

namespace ayla {

TriangleMeshSerializer::TriangleMeshSerializer()
	: Serializer<TriangleMesh>([](){ return "kaskdjhgbfvnsdm348"; })
{ }

}