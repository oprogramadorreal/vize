#ifndef VIZE_VOLUME_DOCUMENT_SERIALIZER_HPP
#define VIZE_VOLUME_DOCUMENT_SERIALIZER_HPP

namespace vize {
	class VolumeDocument;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::VolumeDocument& document, const unsigned int version);

}
}

#endif // VIZE_VOLUME_DOCUMENT_SERIALIZER_HPP