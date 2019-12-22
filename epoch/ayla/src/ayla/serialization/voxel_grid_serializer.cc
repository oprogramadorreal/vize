#include "ayla/serialization/voxel_grid_serializer.hpp"
#include "ayla/geometry/voxel_grid.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template <class Archive, class T>
void serialize(Archive& ar, ayla::VoxelGrid<T>& grid, const unsigned int version) {
	ar & make_nvp("cellsIndexer", grid._cellsIndexer);
	ar & make_nvp("voxels", grid._voxels);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::FloatVoxelGrid, AYLA_API);
EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::ByteVoxelGrid, AYLA_API);

}
}