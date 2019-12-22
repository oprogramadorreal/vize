#ifndef AYLA_REGULAR_GRID_INDEXER_SERIALIZER_HPP
#define AYLA_REGULAR_GRID_INDEXER_SERIALIZER_HPP

namespace ayla {
	class RegularGridIndexer;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::RegularGridIndexer& indexer, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, ayla::RegularGridIndexer* indexer, const unsigned int version);

}
}

#endif // AYLA_REGULAR_GRID_INDEXER_SERIALIZER_HPP