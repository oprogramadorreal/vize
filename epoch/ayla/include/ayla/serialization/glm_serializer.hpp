#ifndef AYLA_GLM_SERIALIZER_HPP
#define AYLA_GLM_SERIALIZER_HPP

#include <glm/glm.hpp>

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive &ar, glm::vec2& vector, const unsigned int version);

template <class Archive>
void serialize(Archive &ar, glm::vec3& vector, const unsigned int version);

template <class Archive>
void serialize(Archive &ar, glm::vec4& vector, const unsigned int version);

template <class Archive>
void serialize(Archive &ar, glm::mat4& matrix, const unsigned int version);

}
}

#endif // AYLA_GLM_SERIALIZER_HPP
