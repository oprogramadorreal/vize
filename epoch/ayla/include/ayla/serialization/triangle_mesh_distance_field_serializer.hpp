#ifndef AYLA_TRIANGLE_MESH_DISTANCE_FIELD_SERIALIZER_HPP
#define AYLA_TRIANGLE_MESH_DISTANCE_FIELD_SERIALIZER_HPP

namespace ayla {
	class TriangleMeshDistanceField;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::TriangleMeshDistanceField& meshDF, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, ayla::TriangleMeshDistanceField* meshDF, const unsigned int version);

}
}

#include "ayla/serialization/serializer.hpp"

namespace ayla {

/**
 * Knows how to serialize a triangle mesh distance field.
 *
 * @author O Programador
 */
class AYLA_API TriangleMeshDistanceFieldSerializer final : public Serializer<TriangleMeshDistanceField> {
public:
	TriangleMeshDistanceFieldSerializer();
};

}

#endif // AYLA_TRIANGLE_MESH_DISTANCE_FIELD_SERIALIZER_HPP