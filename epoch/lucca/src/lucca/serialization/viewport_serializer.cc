#include "lucca/serialization/viewport_serializer.hpp"
#include "lucca/viewport.hpp"

#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, lucca::Viewport& viewport, const unsigned int version) {
	ar & make_nvp("viewportRelativeX", viewport._relativeX);
	ar & make_nvp("viewportRelativeY", viewport._relativeY);
	ar & make_nvp("viewportRelativeWidth", viewport._relativeWidth);
	ar & make_nvp("viewportRelativeHeight", viewport._relativeHeight);
}

template<class Archive>
void load_construct_data(Archive& ar, lucca::Viewport* viewport, const unsigned int version) {
	::new(viewport)lucca::Viewport();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(lucca::Viewport, LUCCA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(lucca::Viewport, LUCCA_API);

}
}