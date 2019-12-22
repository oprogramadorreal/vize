#ifndef VIZE_CONTROL_POINT_SERIALIZER_HPP
#define VIZE_CONTROL_POINT_SERIALIZER_HPP

namespace vize {
	class ControlPoint;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::ControlPoint& point, const unsigned int version);

}
}

#endif // VIZE_CONTROL_POINT_SERIALIZER_HPP