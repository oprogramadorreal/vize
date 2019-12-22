#ifndef AYLA_GL_CAMERA_TO_VIEWPORT_RAY_HPP
#define AYLA_GL_CAMERA_TO_VIEWPORT_RAY_HPP

#include "ayla_gl/config.hpp"

#include <ayla/geometry/ray.hpp>

namespace ayla_gl {

/**
 * @param x_ndc Position x coordinate in Normalized Device Coordinates (NDC) [-1, 1]
 * @param y_ndc Position y coordinate in Normalized Device Coordinates (NDC) [-1, 1]
 */
AYLA_GL_API ayla::Ray getCameraToViewportRay(float x_ndc, float y_ndc, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);

inline ayla::Ray getCameraToViewportRay(const glm::vec2& position_ndc, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
	return getCameraToViewportRay(position_ndc.x, position_ndc.y, projectionMatrix, viewMatrix);
}

}

#endif // AYLA_GL_CAMERA_TO_VIEWPORT_RAY_HPP