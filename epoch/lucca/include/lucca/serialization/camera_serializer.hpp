#ifndef LUCCA_CAMERA_SERIALIZER_HPP
#define LUCCA_CAMERA_SERIALIZER_HPP

namespace lucca {
	class Camera;
}

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, lucca::Camera& camera, const unsigned int version);

template<class Archive>
void load_construct_data(Archive& ar, lucca::Camera* camera, const unsigned int version);

}
}

#endif // LUCCA_CAMERA_SERIALIZER_HPP