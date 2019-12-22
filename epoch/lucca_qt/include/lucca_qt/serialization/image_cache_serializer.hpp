#ifndef LUCCA_QT_IMAGE_CACHE_SERIALIZER_HPP
#define LUCCA_QT_IMAGE_CACHE_SERIALIZER_HPP

namespace lucca_qt {
	class ImageCache;
}

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive& ar, lucca_qt::ImageCache& imageCache, unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, lucca_qt::ImageCache* imageCache, const unsigned int version);

}
}

#endif // LUCCA_QT_IMAGE_CACHE_SERIALIZER_HPP