#include "ayla/serialization/plane_serializer.hpp"

#include "ayla/geometry/plane.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Plane& plane, const unsigned int version) {
	ar & make_nvp("planeN", plane._n);
	ar & make_nvp("planeD", plane._d);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::Plane, AYLA_API);

}
}