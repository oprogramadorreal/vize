#ifndef VIZE_VOLUME_PAINTER_SERIALIZER_HPP
#define VIZE_VOLUME_PAINTER_SERIALIZER_HPP

namespace vize {
	class VolumePainter;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::VolumePainter& painter, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, vize::VolumePainter* painter, const unsigned int version);

}
}

#endif // VIZE_VOLUME_PAINTER_SERIALIZER_HPP