#include "ayla/serialization/ray_serializer.hpp"

#include "ayla/geometry/ray.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Ray& ray, const unsigned int version) {
	ar & make_nvp("rayOrigin", ray._origin);
	ar & make_nvp("rayDirection", ray._direction);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::Ray, AYLA_API);

}
}