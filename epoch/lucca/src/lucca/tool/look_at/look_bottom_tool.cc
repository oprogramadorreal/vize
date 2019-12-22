#include "lucca/tool/look_at/look_bottom_tool.hpp"

#include "lucca/view/view.hpp"
#include "lucca/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace lucca {

LookBottomTool::LookBottomTool(View& view)
	: LookAtTool(view)
{ }

LookBottomTool::~LookBottomTool() = default;

glm::mat4 LookBottomTool::_calculateViewMatrix(Float distance, const glm::vec3& target) const {
	return glm::lookAt(target + glm::vec3(0, -distance, 0.2f), target, glm::vec3(0, 0, 1));
}

}