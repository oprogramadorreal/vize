#include "lucca/tool/look_at/look_top_tool.hpp"

#include "lucca/view/view.hpp"
#include "lucca/camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace lucca {

LookTopTool::LookTopTool(View& view)
	: LookAtTool(view)
{ }

LookTopTool::~LookTopTool() = default;

glm::mat4 LookTopTool::_calculateViewMatrix(Float distance, const glm::vec3& target) const {
	return glm::lookAt(target + glm::vec3(0, distance, 0.2f), target, glm::vec3(0, 0, -1));
}

}