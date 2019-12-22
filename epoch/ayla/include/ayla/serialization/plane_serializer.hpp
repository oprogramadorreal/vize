#ifndef AYLA_PLANE_SERIALIZER_HPP
#define AYLA_PLANE_SERIALIZER_HPP

namespace ayla {
	class Plane;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Plane& plane, const unsigned int version);

}
}

#endif // AYLA_PLANE_SERIALIZER_HPP