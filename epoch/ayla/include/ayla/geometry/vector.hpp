#ifndef AYLA_VECTOR_HPP
#define AYLA_VECTOR_HPP

#include "ayla/config.hpp"

#include <glm/gtc/epsilon.hpp>

#include <ostream>

namespace ayla {

/**
 * Compare two glm::vec3.
 */
inline bool epsilonEqual(const glm::vec3& a, const glm::vec3& b, Float epsilon = std::numeric_limits<Float>::epsilon()) {
	return glm::epsilonEqual(a.x, b.x, epsilon) && glm::epsilonEqual(a.y, b.y, epsilon) && glm::epsilonEqual(a.z, b.z, epsilon);
}
inline bool epsilonNotEqual(const glm::vec3& a, const glm::vec3& b, Float epsilon = std::numeric_limits<Float>::epsilon()) {
	return !epsilonEqual(a, b, epsilon);
}

inline bool isnan(const glm::vec3& v) {
	return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z);
}
inline bool isinf(const glm::vec3& v) {
	return std::isinf(v.x) || std::isinf(v.y) || std::isinf(v.z);
}
inline bool isinf(const glm::vec2& v) {
	return std::isinf(v.x) || std::isinf(v.y);
}

/**
 * @return A vector that is orthogonal (perpendicular) to @param r.
 *
 * Find the smallest component (in absolute value) of vector r, and set it to 0.
 * Swap the two remaining components, and then negate the first of these.
 *
 * Described in "Real-Time Rendering" (third edition), page 71.
 */
AYLA_API glm::vec3 orthogonal(const glm::vec3& r);

AYLA_API glm::vec2 transformPosition(const glm::mat3& transform, const glm::vec2& pos);
AYLA_API glm::vec3 transformPosition(const glm::mat4& transform, const glm::vec3& pos);
AYLA_API glm::vec2 transformDirection(const glm::mat3& transform, const glm::vec2& pos);
AYLA_API glm::vec3 transformDirection(const glm::mat4& transform, const glm::vec3& pos);

}

namespace std {

AYLA_API std::ostream& operator<<(ostream &out, const glm::vec3& v);

}

#endif // AYLA_VECTOR_HPP