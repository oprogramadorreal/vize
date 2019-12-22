#include "lucca/tool/look_at/look_diagonal_tool.hpp"

#include "lucca/view/view.hpp"
#include "lucca/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace lucca {

LookDiagonalTool::LookDiagonalTool(View& view)
	: LookAtTool(view)
{ }

LookDiagonalTool::~LookDiagonalTool() = default;

glm::mat4 LookDiagonalTool::_calculateViewMatrix(Float distance, const glm::vec3& target) const {
	return glm::lookAt(target + glm::vec3(distance, distance, distance) / Float(std::sqrt(3.0f)), target, glm::vec3(0, 1, 0));
}

}