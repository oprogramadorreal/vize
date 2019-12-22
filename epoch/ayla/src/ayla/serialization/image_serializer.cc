#include "ayla/serialization/image_serializer.hpp"

#include "ayla/image.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::Image& image, const unsigned int version) {
	ar & make_nvp("imageWidth", image._width);
	ar & make_nvp("imageHeight", image._height);
	ar & make_nvp("imageBytes", image._bytes);
}

template<class Archive>
void load_construct_data(Archive& ar, ayla::Image* image, const unsigned int version) {
	::new(image)ayla::Image();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::Image, AYLA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(ayla::Image, AYLA_API);

}
}