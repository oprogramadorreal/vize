#ifndef LUCCA_IMAGE_CACHE_HPP
#define LUCCA_IMAGE_CACHE_HPP

#include "lucca_qt/config.hpp"

#include "lucca_qt/serialization/image_cache_serializer.hpp"

#include <boost/noncopyable.hpp>

#include <unordered_map>

namespace ayla {
	class Image;
}

namespace lucca_qt {

/**
 * @author O Programador
 */
class LUCCA_QT_API ImageCache final : boost::noncopyable {
public:
	ImageCache();
	~ImageCache();

	std::shared_ptr<ayla::Image> getImage(const std::string& fileName);

private:
	using MapType = std::unordered_map<std::string, std::shared_ptr<ayla::Image> >;
	MapType _images;

	template <class Archive> friend void boost::serialization::serialize(Archive&, lucca_qt::ImageCache&, const unsigned int);
	template <class Archive> friend void boost::serialization::load_construct_data(Archive&, lucca_qt::ImageCache*, const unsigned int);
};

}

#endif // LUCCA_IMAGE_CACHE_HPP