#include "lucca/event/trackball_camera_controller.hpp"

#include "lucca/event/mouse_event.hpp"
#include "lucca/camera.hpp"

#include <glm/gtx/norm.hpp>

namespace lucca {

TrackballCameraController::TrackballCameraController(lucca::Camera& camera)
	: _camera(camera)
{
	_createCameraConnection();

	_setTrackball();
	_updateCamera();

	setRotationButton(MouseButton::Right);
}

void TrackballCameraController::copyCameraFrom(const TrackballCameraController& other) {
	_trackball = other._trackball;
	_updateCamera();
}

void TrackballCameraController::_createCameraConnection() {
	_cameraConnection = _camera.connectToChangedSignal(
		[this](const Camera&) {
			_setTrackball();
		}
	);
}

void TrackballCameraController::_updateCamera() const {
	boost::signals2::shared_connection_block cameraSignalBlock(_cameraConnection); // ignore camera signals
	_camera.setViewMatrix(_trackball.getViewMatrix());
	_camera.postRedisplay();
}

void TrackballCameraController::_setTrackball() {
	const auto epsilon = Float(0.000001f);
	
	const auto cameraTarget = _getCameraTarget();
	const auto cameraPosition = _camera.getPosition();
	
	_trackball.setTarget(cameraTarget);
	
	if (!ayla::isZero(glm::distance2(cameraTarget, cameraPosition), epsilon)) {
		_trackball.setPosition(cameraPosition);
	}
}

void TrackballCameraController::setCameraTarget(const glm::vec3& target) {
	setCameraTarget(
		[&target]{ return target; }
	);
}

void TrackballCameraController::setCameraTarget(std::function<glm::vec3()> targetFunction) {
	_cameraTargetFunction = targetFunction;
}

void TrackballCameraController::setRotationButton(MouseButton button) {
	_rotationButtons.clearFlags();
	addRotationButton(button);
}

void TrackballCameraController::addRotationButton(MouseButton button) {
	_rotationButtons.addFlag(button);
}

glm::vec3 TrackballCameraController::_getCameraTarget() const {
	return _cameraTargetFunction ? _cameraTargetFunction() : glm::vec3(0.0f);
}

Float TrackballCameraController::_getRotationSpeed() const {
	return Float(0.005f);
}

Float TrackballCameraController::_getTranslationSpeed() const {
	return Float(0.01f);
}

EventSignalResult TrackballCameraController::_mousePressEvent(const MouseEvent& e) {
	_lastMousePosition = e.getPosition();
	return EventSignalResult::defaultValue();
}

EventSignalResult TrackballCameraController::_mouseReleaseEvent(const MouseEvent& e) {
	_lastMousePosition = e.getPosition();
	return EventSignalResult::defaultValue();
}

namespace {
	Float _manhattanLength(const glm::vec2& vector) {
		return std::abs(vector.x) + std::abs(vector.y);
	}
}

EventSignalResult TrackballCameraController::_mouseMoveEvent(const MouseEvent& e) {
	const auto mousePosition = e.getPosition();
	const auto mousePositionDiff = mousePosition - _lastMousePosition;
	_lastMousePosition = mousePosition;
	
	const auto maxDiffLength = Float(50);

	if (_manhattanLength(mousePositionDiff) < maxDiffLength) {
		if (_rotationButtons.testFlag(e.getButton())) {
			const auto speed = _getRotationSpeed();
			_trackball.addPitch(mousePositionDiff.y * -speed);
			_trackball.addYaw(mousePositionDiff.x * -speed);
			_updateCamera();
	
		} else if (e.getButton() == MouseButton::Middle) {
			const auto speed = _getTranslationSpeed();
			const auto epsilon = Float(0.001f);
			const auto up = _trackball.getUp();
			const auto right = _trackball.getRight();
	
			if (!ayla::isZero(glm::length2(up), epsilon) && !ayla::isZero(glm::length2(right), epsilon)) {
				const auto upMove = glm::normalize(up) * (mousePositionDiff.y * -speed);
				const auto rightMove = glm::normalize(right) * (mousePositionDiff.x * speed);
	
				const auto newTarget = _trackball.getTarget() + rightMove + upMove;
	
				_trackball.setTarget(newTarget);
			}
	
			_updateCamera();
		}
	}

	return EventSignalResult::defaultValue();
}

EventSignalResult TrackballCameraController::_wheelEvent(const MouseWheelEvent& e) {
	_trackball.setRadius(_trackball.getRadius() + Float(e.getMovement()) * -_getTranslationSpeed());
	_updateCamera();
	return EventSignalResult::defaultValue();
}

MousePressedSignalListener TrackballCameraController::_createMousePressedListener() {
	return std::bind(&TrackballCameraController::_mousePressEvent, this, std::placeholders::_1);
}
MouseReleasedSignalListener TrackballCameraController::_createMouseReleasedListener() {
	return std::bind(&TrackballCameraController::_mouseReleaseEvent, this, std::placeholders::_1);
}
MouseMovedSignalListener TrackballCameraController::_createMouseMovedListener() {
	return std::bind(&TrackballCameraController::_mouseMoveEvent, this, std::placeholders::_1);
}
MouseWheelSignalListener TrackballCameraController::_createMouseWheelListener() {
	return std::bind(&TrackballCameraController::_wheelEvent, this, std::placeholders::_1);
}

}