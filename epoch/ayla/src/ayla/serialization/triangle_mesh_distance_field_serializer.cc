#include "ayla/serialization/triangle_mesh_distance_field_serializer.hpp"
#include "ayla/geometry/triangle_mesh_distance_field.hpp"
#include "ayla/geometry/regular_grid_indexer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::TriangleMeshDistanceField& meshDF, const unsigned int version) {
	ar & make_nvp("mesh", meshDF._mesh);
	ar & make_nvp("voxelGrid", meshDF._voxelGrid);
}

template<class Archive>
void load_construct_data(Archive& ar, ayla::TriangleMeshDistanceField* meshDF, const unsigned int version) {
	::new(meshDF)ayla::TriangleMeshDistanceField();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::TriangleMeshDistanceField, AYLA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(ayla::TriangleMeshDistanceField, AYLA_API);

}
}

namespace ayla {

TriangleMeshDistanceFieldSerializer::TriangleMeshDistanceFieldSerializer()
	: Serializer<TriangleMeshDistanceField>([](){ return "4387fg8j3f92kd8s"; })
{ }

}