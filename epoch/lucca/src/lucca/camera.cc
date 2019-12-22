#include "lucca/camera.hpp"

#include <ayla_gl/world_to_ndc.hpp>
#include <ayla_gl/camera_to_viewport_ray.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace lucca {

Camera::Camera() = default;

Camera::Camera(const Viewport& viewport)
	: _viewport(viewport)
{ }

Camera::Camera(const Camera& other)
	: Camera(other._viewport)
{
	_viewMatrix = other._viewMatrix;
	_fovyRadians = other._fovyRadians;
	_nearDistance = other._nearDistance;
	_farDistance = other._farDistance;
}

Camera::~Camera() = default;

glm::mat4 Camera::getViewMatrix() const {
	return _viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
	auto viewportHeight = Float(_viewport.getAbsoluteHeight());

	if (ayla::isZero(viewportHeight)) {
		viewportHeight = Float(1.0f);
	}

	const auto aspect = Float(_viewport.getAbsoluteWidth()) / viewportHeight;

	return glm::perspective(_fovyRadians, aspect, _nearDistance, _farDistance);
}

void Camera::setViewMatrix(const glm::mat4& viewMatrix) {
	_viewMatrix = viewMatrix;
	_signalChanged(*this);
}

void Camera::setProjectionMatrix(Float fovyRadians, Float nearDistance, Float farDistance) {
	_fovyRadians = fovyRadians;
	_nearDistance = nearDistance;
	_farDistance = farDistance;
	_signalChanged(*this);
}

glm::vec3 Camera::getPosition() const {
	const auto inverseViewMatrix = glm::inverse(_viewMatrix);
	return glm::vec3(glm::column(inverseViewMatrix, 3)); // extract position
}

glm::vec3 Camera::getDirection() const {
	// TODO: not sure if "normalize" is necessary
	return glm::normalize(-glm::vec3(glm::row(_viewMatrix, 2))); // extract direction
}

Float Camera::getFovy() const {
	return _fovyRadians;
}

Float Camera::getNearDistance() const {
	return _nearDistance;
}

Float Camera::getFarDistance() const {
	return _farDistance;
}

void Camera::setRenderTarget(RenderTarget* renderTarget) {
	_viewport.setRenderTarget(renderTarget);
}

SizeType Camera::getDefaultFramebufferObject() const {
	return _viewport.getDefaultFramebufferObject();
}

void Camera::postRedisplay() {
	_viewport.postRedisplay();
}

SizeType Camera::getViewportAbsoluteX() const {
	return _viewport.getAbsoluteX();
}

SizeType Camera::getViewportAbsoluteY() const {
	return _viewport.getAbsoluteY();
}

SizeType Camera::getViewportAbsoluteWidth() const {
	return _viewport.getAbsoluteWidth();
}

SizeType Camera::getViewportAbsoluteHeight() const {
	return _viewport.getAbsoluteHeight();
}

ayla::Ray Camera::getCameraToViewportRay(const glm::vec2& pointOnViewport) const {
	const auto ndcPoint = ayla_gl::normalizedToNDC(getNormalizedPointOnViewport(pointOnViewport), true);

	return ayla_gl::getCameraToViewportRay(
		ndcPoint, getProjectionMatrix(), getViewMatrix()
	);
}

glm::vec2 Camera::getNormalizedPointOnViewport(const glm::vec2& pointOnViewport) const {
	const auto viewportWidth = Float(_viewport.getAbsoluteWidth());
	const auto viewportHeight = Float(_viewport.getAbsoluteHeight());
	
	if (ayla::isZero(viewportWidth) || ayla::isZero(viewportHeight)) {
		return glm::vec2(0.0f);
	}
	
	return glm::vec2(
		pointOnViewport.x / viewportWidth,
		pointOnViewport.y / viewportHeight
	);
}

boost::signals2::connection Camera::connectToChangedSignal(ChangedSignal::slot_function_type slot) {
	return _signalChanged.connect(slot);
}

}