#ifndef LUCCA_VIEWPORT_SERIALIZER_HPP
#define LUCCA_VIEWPORT_SERIALIZER_HPP

namespace lucca {
	class Viewport;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, lucca::Viewport& viewport, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, lucca::Viewport* viewport, const unsigned int version);

}
}

#endif // LUCCA_VIEWPORT_SERIALIZER_HPP