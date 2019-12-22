#include "lucca/serialization/camera_serializer.hpp"
#include "lucca/camera.hpp"

#include <ayla/serialization/glm_serializer.hpp>
#include <ayla/serialization/boost/explicit_instantiation_macros.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, lucca::Camera& camera, const unsigned int version) {
	ar & make_nvp("cameraViewport", camera._viewport);
	ar & make_nvp("cameraViewMatrix", camera._viewMatrix);
	ar & make_nvp("cameraFovyRadians", camera._fovyRadians);
	ar & make_nvp("cameraNearDistance", camera._nearDistance);
	ar & make_nvp("cameraFarDistance", camera._farDistance);

	if (!Archive::is_saving::value) {
		camera._signalChanged(camera);
	}
}

template<class Archive>
void load_construct_data(Archive& ar, lucca::Camera* camera, const unsigned int version) {
	::new(camera)lucca::Camera();
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(lucca::Camera, LUCCA_API);
EXPLICIT_INSTANTIATION_LOAD_CONSTRUCT_DATA_FUNC_P(lucca::Camera, LUCCA_API);

}
}