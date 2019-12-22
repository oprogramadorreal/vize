#ifndef AYLA_MATRIX_HPP
#define AYLA_MATRIX_HPP

#include "ayla/config.hpp"

#include <ostream>

namespace ayla {

/**
 * Compares the elements of two matrices using glm::epsilonEqual.
 */
AYLA_API bool epsilonEqual(const glm::mat4& a, const glm::mat4& b, Float epsilon);

/**
 * Compares the elements of two matrices using glm::epsilonEqual.
 */
AYLA_API bool epsilonEqual(const glm::mat3& a, const glm::mat3& b, Float epsilon);

}

namespace std {

AYLA_API ostream& operator<<(ostream& stream, const glm::mat4& a);
AYLA_API ostream& operator<<(ostream& stream, const glm::mat3& a);

}

#endif // AYLA_MATRIX_HPP