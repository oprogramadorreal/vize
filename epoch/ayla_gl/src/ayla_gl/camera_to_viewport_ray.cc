#include "ayla_gl/camera_to_viewport_ray.hpp"

namespace ayla_gl  {

ayla::Ray getCameraToViewportRay(float x_ndc, float y_ndc, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
	const glm::vec4 rayStart_ndc(x_ndc, y_ndc, -1.0f, 1.0f); // near plane is at Z = -1 in NDC
	const glm::vec4 rayEnd_ndc(x_ndc, y_ndc, 1.0f, 1.0f);

	const glm::mat4 invVP = glm::inverse(projectionMatrix * viewMatrix);

	glm::vec4 rayBegin_worldspace = invVP * rayStart_ndc;
	rayBegin_worldspace /= rayBegin_worldspace.w;

	glm::vec4 rayEnd_worldspace = invVP * rayEnd_ndc;
	rayEnd_worldspace /= rayEnd_worldspace.w;

	glm::vec3 rayDirection(rayEnd_worldspace - rayBegin_worldspace);
	rayDirection = glm::normalize(rayDirection);

	return ayla::Ray(glm::vec3(rayBegin_worldspace), rayDirection);
}

}