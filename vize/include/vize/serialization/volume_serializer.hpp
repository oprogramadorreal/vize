#ifndef VIZE_VOLUME_SERIALIZER_HPP
#define VIZE_VOLUME_SERIALIZER_HPP

namespace vize {
	class Volume;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::Volume& volume, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, vize::Volume* volume, const unsigned int version);

}
}

#endif // VIZE_VOLUME_SERIALIZER_HPP