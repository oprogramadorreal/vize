#ifndef AYLA_QUATERNION_HPP
#define AYLA_QUATERNION_HPP

#include "ayla/config.hpp"

#include <glm/gtc/quaternion.hpp>

#include <ostream>

namespace std {

AYLA_API std::ostream& operator<<(ostream &out, const glm::quat& q);

}

#endif // AYLA_QUATERNION_HPP