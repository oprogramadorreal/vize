#ifndef VIZE_TRANSFER_FUNCTION_SERIALIZER_HPP
#define VIZE_TRANSFER_FUNCTION_SERIALIZER_HPP

namespace vize {
	class TransferFunction;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::TransferFunction& transferFunc, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, vize::TransferFunction* transferFunc, const unsigned int version);

}
}

#endif // VIZE_TRANSFER_FUNCTION_SERIALIZER_HPP