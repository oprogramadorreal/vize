#ifndef AYLA_IMAGE_SERIALIZER_HPP
#define AYLA_IMAGE_SERIALIZER_HPP

namespace ayla {
	class Image;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Image& image, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, ayla::Image* image, const unsigned int version);

}
}

#endif // AYLA_IMAGE_SERIALIZER_HPP