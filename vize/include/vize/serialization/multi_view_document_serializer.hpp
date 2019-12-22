#ifndef VIZE_MULTI_VIEW_DOCUMENT_SERIALIZER_HPP
#define VIZE_MULTI_VIEW_DOCUMENT_SERIALIZER_HPP

#include <boost/serialization/split_free.hpp>

namespace lucca {
	class MultiViewDocument;
}

namespace boost {
namespace serialization {

template<class Archive>
void load(Archive& ar, lucca::MultiViewDocument& document, unsigned int version);

template<class Archive>
void save(Archive& ar, const lucca::MultiViewDocument& document, unsigned int version);

}
}

BOOST_SERIALIZATION_SPLIT_FREE(lucca::MultiViewDocument);

#endif // VIZE_MULTI_VIEW_DOCUMENT_SERIALIZER_HPP