#include "lucca/tool/look_at/look_at_tool.hpp"

#include "lucca/camera.hpp"
#include "lucca/view/view.hpp"
#include "lucca/painter/painter.hpp"

#include <ayla/geometry/axis_aligned_box.hpp>

namespace lucca {

LookAtTool::LookAtTool(View& view)
	: _view(view)
{ }

LookAtTool::~LookAtTool() = default;

glm::vec3 LookAtTool::getCameraTarget() const {
	if (const auto painter = _view.getPainter()) {
		return painter->getAABB().getCenter();
	}

	return glm::vec3(0.0f);
}

Float LookAtTool::_calculateBestCameraDistance(const Camera& camera) const {
	auto distance = Float(10.0f);

	if (const auto painter = _view.getPainter()) {
		const auto angle = Float(camera.getFovy() / 2.0f);
		const auto aabbExtensions = painter->getAABB().getExtensions();
		const auto h = Float(std::max(std::max(aabbExtensions.x, aabbExtensions.y), aabbExtensions.z));
		const auto tangent = std::tan(angle);

		if (!ayla::isZero(tangent)) {
			distance = 0.75f * (h / tangent);
		}
	}

	return distance;
}

void LookAtTool::_activateImpl() {
	if (auto camera = _view.getDefaultCamera()) {
		const auto distance = _calculateBestCameraDistance(*camera);
		const auto target = getCameraTarget();

		const auto viewMatrix = _calculateViewMatrix(distance, target);

		camera->setViewMatrix(viewMatrix);
		camera->postRedisplay();
	}
}

}