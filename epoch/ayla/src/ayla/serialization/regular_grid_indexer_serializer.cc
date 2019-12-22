#include "ayla/serialization/regular_grid_indexer_serializer.hpp"

#include "ayla/geometry/regular_grid_indexer.hpp"

#include "ayla/serialization/glm_serializer.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::RegularGridIndexer& indexer, const unsigned int version) {
	ar & make_nvp("aabb", indexer._aabb);
	ar & make_nvp("width", indexer._width);
	ar & make_nvp("height", indexer._height);
	ar & make_nvp("depth", indexer._depth);
}

template<class Archive>
void load_construct_data(Archive& ar, ayla::RegularGridIndexer* indexer, const unsigned int version) {
	::new(indexer)ayla::RegularGridIndexer();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::RegularGridIndexer, AYLA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(ayla::RegularGridIndexer, AYLA_API);

}
}