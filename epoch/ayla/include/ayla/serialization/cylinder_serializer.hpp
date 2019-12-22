#ifndef AYLA_CYLINDER_SERIALIZER_HPP
#define AYLA_CYLINDER_SERIALIZER_HPP

namespace ayla {
	class Cylinder;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Cylinder& cylinder, const unsigned int version);

}
}

#endif // AYLA_CYLINDER_SERIALIZER_HPP