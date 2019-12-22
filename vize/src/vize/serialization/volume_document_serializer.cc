#include "vize/serialization/volume_document_serializer.hpp"

#include "vize/serialization/multi_view_document_serializer.hpp"
#include "vize/document/volume_document.hpp"
#include "vize/model/volume.hpp"

#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>
#include <boost/serialization/shared_ptr.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, vize::VolumeDocument& document, const unsigned int version) {
	ar & make_nvp("volume", document._volume);
	ar & make_nvp("document_name", document._documentName);
	ar & make_nvp("multi_view", static_cast<lucca::MultiViewDocument&>(document));
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(vize::VolumeDocument);

}
}