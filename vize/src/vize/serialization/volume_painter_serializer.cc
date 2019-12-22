#include "vize/serialization/volume_painter_serializer.hpp"

#include "vize/painter/volume_painter.hpp"
#include "vize/model/volume.hpp"

#include <ayla/serialization/glm_serializer.hpp>
#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

#include <boost/serialization/shared_ptr.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::VolumePainter& painter, const unsigned int version) {
	ar & make_nvp("volume", painter._volume);
	ar & make_nvp("transferFunction", painter._transferFunction);
	ar & make_nvp("xPlanePosition", painter._xPlanePosition);
	ar & make_nvp("yPlanePosition", painter._yPlanePosition);
	ar & make_nvp("zPlanePosition", painter._zPlanePosition);
	ar & make_nvp("cuttingPlanesEnabled", painter._cuttingPlanesEnabled);
	ar & make_nvp("lightingEnabled", painter._lightingEnabled);
	ar & make_nvp("samplesStep", painter._samplesStep);
}

template<class Archive>
void load_construct_data(Archive& ar, vize::VolumePainter* painter, const unsigned int version) {
	::new(painter)vize::VolumePainter();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(vize::VolumePainter);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC(vize::VolumePainter);

}
}