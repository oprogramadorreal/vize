#ifndef AYLA_TRIANGULAR_FACE_SERIALIZER_HPP
#define AYLA_TRIANGULAR_FACE_SERIALIZER_HPP

namespace ayla {
	class TriangularFace;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::TriangularFace& face, const unsigned int version);

}
}

#endif // AYLA_TRIANGULAR_FACE_SERIALIZER_HPP