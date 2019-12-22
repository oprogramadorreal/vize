#include "lucca_qt/serialization/image_cache_serializer.hpp"

#include "lucca_qt/image/image_cache.hpp"

#include <ayla/image.hpp>
#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unordered_map.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, lucca_qt::ImageCache& imageCache, unsigned int version) {
	ar & make_nvp("cachedImages", imageCache._images);
}

template<class Archive>
void load_construct_data(Archive& ar, lucca_qt::ImageCache* imageCache, const unsigned int version) {
	::new(imageCache)lucca_qt::ImageCache();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(lucca_qt::ImageCache, LUCCA_QT_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(lucca_qt::ImageCache, LUCCA_QT_API);

}
}