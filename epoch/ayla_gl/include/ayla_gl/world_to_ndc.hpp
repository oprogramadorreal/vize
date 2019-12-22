#ifndef AYLA_GL_WORLD_TO_NDC_HPP
#define AYLA_GL_WORLD_TO_NDC_HPP

#include "ayla_gl/config.hpp"

namespace ayla_gl {

/**
 * @param pointWorld A position in world space.
 *
 * @return A position in Normalized Device Coordinates (NDC) [-1, 1].
 */
AYLA_GL_API glm::vec2 worldToNDC(const glm::vec3& pointWorld, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

/**
 * @param position Normalized position [0, 1]
 */
inline glm::vec2 normalizedToNDC(const glm::vec2& position, bool invertY = true) {
	return (glm::vec2(-1.0f, -1.0f) + position * 2.0f) * glm::vec2(1.0f, invertY ? -1.0f : 1.0f);
}

}

#endif // AYLA_GL_WORLD_TO_NDC_HPP