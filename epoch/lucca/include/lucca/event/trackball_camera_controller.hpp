#ifndef LUCCA_TRACKBALL_CAMERA_CONTROLLER_HPP
#define LUCCA_TRACKBALL_CAMERA_CONTROLLER_HPP

#include "lucca/config.hpp"

#include "lucca/event/event_handler.hpp"
#include "lucca/event/mouse_event.hpp"

#include <ayla_gl/trackball_camera.hpp>

namespace lucca {

class Camera;

/**
 * Controls a lucca::Camera as a "trackball".
 *
 * @author O Programador
 */
class LUCCA_API TrackballCameraController final : public EventHandler {
public:
	TrackballCameraController(lucca::Camera& camera);

	void copyCameraFrom(const TrackballCameraController& other);

	void setCameraTarget(const glm::vec3& target);
	void setCameraTarget(std::function<glm::vec3()> targetFunction);

	void setRotationButton(MouseButton button);
	void addRotationButton(MouseButton button);

private:
	EventSignalResult _mousePressEvent(const MouseEvent& e);
	EventSignalResult _mouseReleaseEvent(const MouseEvent& e);
	EventSignalResult _mouseMoveEvent(const MouseEvent& e);
	EventSignalResult _wheelEvent(const MouseWheelEvent& e);

private:
	void _updateCamera() const;
	void _setTrackball();

	glm::vec3 _getCameraTarget() const;

	Float _getRotationSpeed() const;
	Float _getTranslationSpeed() const;

	void _createCameraConnection();

private:
	virtual MousePressedSignalListener _createMousePressedListener() final override;
	virtual MouseReleasedSignalListener _createMouseReleasedListener() final override;
	virtual MouseMovedSignalListener _createMouseMovedListener() final override;
	virtual MouseWheelSignalListener _createMouseWheelListener() final override;

private:
	lucca::Camera& _camera;
	boost::signals2::scoped_connection _cameraConnection;
	glm::vec2 _lastMousePosition = glm::vec2(0.0f);
	ayla_gl::TrackballCamera _trackball = ayla_gl::TrackballCamera(glm::vec3(0, 0, 0), 10.0f);
	std::function<glm::vec3()> _cameraTargetFunction;
	MouseButtonFlags _rotationButtons;
};

}

#endif // LUCCA_TRACKBALL_CAMERA_CONTROLLER_HPP