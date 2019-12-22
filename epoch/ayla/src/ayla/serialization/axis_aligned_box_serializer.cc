#include "ayla/serialization/axis_aligned_box_serializer.hpp"

#include "ayla/geometry/axis_aligned_box.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::AxisAlignedBox& aabb, const unsigned int version) {
	ar & make_nvp("minPoint", aabb._minPt);
	ar & make_nvp("maxPoint", aabb._maxPt);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::AxisAlignedBox, AYLA_API);

}
}