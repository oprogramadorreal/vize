#ifndef AYLA_VERTEX_SERIALIZER_HPP
#define AYLA_VERTEX_SERIALIZER_HPP

namespace ayla {
	class Vertex;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Vertex& vertex, const unsigned int version);

}
}

#endif // AYLA_VERTEX_SERIALIZER_HPP