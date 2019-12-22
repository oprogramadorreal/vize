#ifndef AYLA_AXIS_ALIGNED_BOX_SERIALIZER_HPP
#define AYLA_AXIS_ALIGNED_BOX_SERIALIZER_HPP

namespace ayla {
	class AxisAlignedBox;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::AxisAlignedBox& aabb, const unsigned int version);

}
}

#endif // AYLA_AXIS_ALIGNED_BOX_SERIALIZER_HPP