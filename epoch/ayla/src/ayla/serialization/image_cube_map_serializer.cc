#include "ayla/serialization/image_cube_map_serializer.hpp"

#include "ayla/image_cube_map.hpp"
#include "ayla/image.hpp"
#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"

#include <boost/serialization/unique_ptr.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, ayla::ImageCubeMap& image, const unsigned int version) {
	ar & make_nvp("imagePositiveX", image._images[0]);
	ar & make_nvp("imageNegativeX", image._images[1]);
	ar & make_nvp("imagePositiveY", image._images[2]);
	ar & make_nvp("imageNegativeY", image._images[3]);
	ar & make_nvp("imagePositiveZ", image._images[4]);
	ar & make_nvp("imageNegativeZ", image._images[5]);
}

template<class Archive>
void load_construct_data(Archive& ar, ayla::ImageCubeMap* image, const unsigned int version) {
	::new(image)ayla::ImageCubeMap();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(ayla::ImageCubeMap, AYLA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(ayla::ImageCubeMap, AYLA_API);

}
}