#ifndef AYLA_LINE_SEGMENT_SERIALIZER_HPP
#define AYLA_LINE_SEGMENT_SERIALIZER_HPP

namespace ayla {
	class LineSegment;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::LineSegment& segment, const unsigned int version);

}
}

#endif // AYLA_LINE_SEGMENT_SERIALIZER_HPP