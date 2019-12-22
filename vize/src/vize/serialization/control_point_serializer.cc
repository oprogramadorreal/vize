#include "vize/serialization/control_point_serializer.hpp"
#include "vize/model/transfer_function/control_point.hpp"

#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::ControlPoint& point, const unsigned int version) {
	ar & make_nvp("color", point._color);
	ar & make_nvp("position", point._position);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(vize::ControlPoint);

}
}