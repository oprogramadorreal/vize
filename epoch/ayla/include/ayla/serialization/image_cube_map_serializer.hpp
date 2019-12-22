#ifndef AYLA_IMAGE_CUBE_MAP_SERIALIZER_HPP
#define AYLA_IMAGE_CUBE_MAP_SERIALIZER_HPP

namespace ayla {
	class ImageCubeMap;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::ImageCubeMap& image, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, ayla::ImageCubeMap* image, const unsigned int version);

}
}

#endif // AYLA_IMAGE_CUBE_MAP_SERIALIZER_HPP