#include "ayla/serialization/glm_serializer.hpp"

#include "ayla/serialization/boost/explicit_instantiation_macros.hpp"
#include "ayla/config.hpp"

#include <boost/serialization/nvp.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive &ar, glm::vec2& vector, const unsigned int version) {
	ar & make_nvp("x", vector.x);
	ar & make_nvp("y", vector.y);
}

template<class Archive>
void serialize(Archive &ar, glm::vec3& vector, const unsigned int version) {
	ar & make_nvp("x", vector.x);
	ar & make_nvp("y", vector.y);
	ar & make_nvp("z", vector.z);
}

template<class Archive>
void serialize(Archive &ar, glm::vec4& vector, const unsigned int version) {
	ar & make_nvp("x", vector.x);
	ar & make_nvp("y", vector.y);
	ar & make_nvp("z", vector.z);
	ar & make_nvp("w", vector.w);
}

template <class Archive>
void serialize(Archive &ar, glm::mat4& matrix, const unsigned int version) {
	ar & make_nvp("m00", matrix[0][0]);
	ar & make_nvp("m01", matrix[0][1]);
	ar & make_nvp("m02", matrix[0][2]);
	ar & make_nvp("m03", matrix[0][3]);
	ar & make_nvp("m10", matrix[1][0]);
	ar & make_nvp("m11", matrix[1][1]);
	ar & make_nvp("m12", matrix[1][2]);
	ar & make_nvp("m13", matrix[1][3]);
	ar & make_nvp("m20", matrix[2][0]);
	ar & make_nvp("m21", matrix[2][1]);
	ar & make_nvp("m22", matrix[2][2]);
	ar & make_nvp("m23", matrix[2][3]);
	ar & make_nvp("m30", matrix[3][0]);
	ar & make_nvp("m31", matrix[3][1]);
	ar & make_nvp("m32", matrix[3][2]);
	ar & make_nvp("m33", matrix[3][3]);
}

EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(glm::vec2, AYLA_API);
EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(glm::vec3, AYLA_API);
EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(glm::vec4, AYLA_API);
EXPLICIT_INSTANTIATION_SERIALIZE_FUNC_P(glm::mat4, AYLA_API);

}
}