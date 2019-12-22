#include "ayla/serialization/triangular_face_serializer.hpp"

#include "ayla/geometry/triangular_face.hpp"

#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::TriangularFace& face, const unsigned int version) {
	ar & make_nvp("vertexA", face._va);
	ar & make_nvp("vertexB", face._vb);
	ar & make_nvp("vertexC", face._vc);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::TriangularFace, AYLA_API);

}
}