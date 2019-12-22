#include "ayla/serialization/vertex_serializer.hpp"

#include "ayla/geometry/vertex.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Vertex& vertex, const unsigned int version) {
	ar & make_nvp("position", vertex._position);
	ar & make_nvp("normal", vertex._normal);
	ar & make_nvp("uv", vertex._uv);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::Vertex, AYLA_API);

}
}