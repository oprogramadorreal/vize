#ifndef VIZE_COLOR_SERIALIZER_HPP
#define VIZE_COLOR_SERIALIZER_HPP

namespace vize {
	class Color;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, vize::Color& color, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, vize::Color* color, const unsigned int version);

}
}

#endif // VIZE_COLOR_SERIALIZER_HPP