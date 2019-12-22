#include "ayla/serialization/line_segment_serializer.hpp"

#include "ayla/geometry/line_segment.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::LineSegment& segment, const unsigned int version) {
	ar & make_nvp("lineSegmentP", segment._p);
	ar & make_nvp("lineSegmentQ", segment._q);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::LineSegment, AYLA_API);

}
}