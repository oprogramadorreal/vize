#include "vize/serialization/transfer_function_serializer.hpp"
#include "vize/model/transfer_function/transfer_function.hpp"

#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::TransferFunction& transferFunc, const unsigned int version) {
	ar & make_nvp("controlPoints", transferFunc._controlPoints);
}

template<class Archive>
void load_construct_data(Archive& ar, vize::TransferFunction* transferFunc, const unsigned int version) {
	::new(transferFunc)vize::TransferFunction();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC(vize::TransferFunction);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC(vize::TransferFunction);

}
}