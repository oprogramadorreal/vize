#ifndef AYLA_RAY_SERIALIZER_HPP
#define AYLA_RAY_SERIALIZER_HPP

namespace ayla {
	class Ray;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Ray& ray, const unsigned int version);

}
}

#endif // AYLA_RAY_SERIALIZER_HPP