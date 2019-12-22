#include "lucca/tool/look_at/look_back_tool.hpp"

#include "lucca/view/view.hpp"
#include "lucca/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace lucca {

LookBackTool::LookBackTool(View& view)
	: LookAtTool(view)
{ }

LookBackTool::~LookBackTool() = default;

glm::mat4 LookBackTool::_calculateViewMatrix(Float distance, const glm::vec3& target) const {
	return glm::lookAt(target + glm::vec3(0, 0, -distance), target, glm::vec3(0, 1, 0));
}

}