#include "vize/serialization/color_serializer.hpp"
#include "vize/model/color.hpp"

#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::Color& color, const unsigned int version) {
	ar & make_nvp("r", color._r);
	ar & make_nvp("g", color._g);
	ar & make_nvp("b", color._b);
	ar & make_nvp("a", color._a);
}

template<class Archive>
void load_construct_data(Archive& ar, vize::Color* color, const unsigned int version) {
	::new(color)vize::Color();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(vize::Color);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC(vize::Color);

}
}