#include "ayla_gl/world_to_ndc.hpp"

namespace ayla_gl  {

glm::vec2 worldToNDC(const glm::vec3& pointWorld, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
	glm::vec4 aux = projectionMatrix * viewMatrix * glm::vec4(pointWorld, 1.0f);

	if (!ayla::isZero(aux.w)) {
		aux.x /= aux.w;
		aux.y /= aux.w;
	}

	return glm::vec2(aux);
}

}