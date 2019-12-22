#ifndef AYLA_VOXEL_GRID_SERIALIZER_HPP
#define AYLA_VOXEL_GRID_SERIALIZER_HPP

namespace ayla {
	template <class T>
	class VoxelGrid;
}

namespace boost {
namespace serialization {

template <class Archive, class T>
void serialize(Archive& ar, ayla::VoxelGrid<T>& grid, const unsigned int version);

}
}

#endif // AYLA_VOXEL_GRID_SERIALIZER_HPP