#include "lucca/event/view_event.hpp"

#include "lucca/view/view.hpp"
#include "lucca/camera.hpp"

#include <ayla/geometry/ray.hpp>
#include <ayla_gl/world_to_ndc.hpp>

namespace lucca {

ViewEvent::ViewEvent(Float x, Float y, const View* sourceView, KeyboardModifierFlags keyboardModifier)
	: Event(keyboardModifier), _x(x), _y(y), _sourceView(sourceView)
{ }

ViewEvent::~ViewEvent() = default;
	
glm::vec2 ViewEvent::getPosition() const {
	return glm::vec2(_x, _y);
}

boost::optional<glm::vec2> ViewEvent::getNormalizedPosition() const {
	if (const auto camera = _getSourceCamera()) {
		return camera->getNormalizedPointOnViewport(getPosition());
	}

	return boost::none;
}

boost::optional<glm::vec2> ViewEvent::getNDCPosition(bool invertY) const {
	if (const auto normalizedPosition = getNormalizedPosition()) {
		return ayla_gl::normalizedToNDC(*normalizedPosition, invertY);
	}

	return boost::none;
}

boost::optional<ayla::Ray> ViewEvent::getCameraToViewportRay() const {
	if (const auto camera = _getSourceCamera()) {
		return camera->getCameraToViewportRay(getPosition());
	}

	return boost::none;
}

boost::optional<glm::vec3> ViewEvent::getCameraDirection() const {
	if (const auto camera = _getSourceCamera()) {
		return camera->getDirection();
	}

	return boost::none;
}

boost::optional<glm::mat4> ViewEvent::getViewMatrix() const {
	if (const auto camera = _getSourceCamera()) {
		return camera->getViewMatrix();
	}

	return boost::none;
}

boost::optional<glm::mat4> ViewEvent::getProjectionMatrix() const {
	if (const auto camera = _getSourceCamera()) {
		return camera->getProjectionMatrix();
	}

	return boost::none;
}

const View* ViewEvent::getSourceView() const {
	return _sourceView;
}

const Camera* ViewEvent::_getSourceCamera() const {
	const auto view = getSourceView();

	if (!view) {
		return nullptr;
	}

	// TODO: choose camera based on event position

	return view->getDefaultCamera();
}

}