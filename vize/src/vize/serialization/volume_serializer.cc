#include "vize/serialization/volume_serializer.hpp"
#include "vize/model/volume.hpp"

#include <ayla/serialization/glm_serializer.hpp>
#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::Volume& volume, const unsigned int version) {
	ar & make_nvp("voxelGrid", volume._voxelGrid);
	ar & make_nvp("modelMatrix", volume._modelMatrix);
}

template<class Archive>
void load_construct_data(Archive& ar, vize::Volume* volume, const unsigned int version) {
	::new(volume)vize::Volume();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(vize::Volume);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC(vize::Volume);

}
}