#include "lucca/tool/look_at/look_right_tool.hpp"

#include "lucca/view/view.hpp"
#include "lucca/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace lucca {

LookRightTool::LookRightTool(View& view)
	: LookAtTool(view)
{ }

LookRightTool::~LookRightTool() = default;

glm::mat4 LookRightTool::_calculateViewMatrix(Float distance, const glm::vec3& target) const {
	return glm::lookAt(target + glm::vec3(distance, 0, 0), target, glm::vec3(0, 1, 0));
}

}