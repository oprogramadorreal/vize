#include "ayla/serialization/cylinder_serializer.hpp"

#include "ayla/geometry/cylinder.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Cylinder& cylinder, const unsigned int version) {
	ar & make_nvp("cylinderAxis", cylinder._axis);
	ar & make_nvp("cylinderRadius", cylinder._radius);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::Cylinder, AYLA_API);

}
}