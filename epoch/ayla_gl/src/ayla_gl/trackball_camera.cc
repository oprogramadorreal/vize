#include "ayla_gl/trackball_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace ayla_gl {

TrackballCamera::TrackballCamera(const glm::vec3& target, float radius)
	: _target(target), _radius(radius), _pitch(0.0f), _yaw(0.0f)
{ }

void TrackballCamera::addYaw(float angleRadians) {
	_yaw += angleRadians;
}

void TrackballCamera::addPitch(float angleRadians) {
	_pitch += angleRadians;
	_pitch = glm::clamp(_pitch, -glm::half_pi<float>()*0.999f, glm::half_pi<float>()*0.999f);
}

void TrackballCamera::setPitch(float angleRadians) {
	_pitch = glm::clamp( angleRadians, -glm::half_pi<float>()*0.999f, glm::half_pi<float>()*0.999f);
}

void TrackballCamera::setYaw(float angleRadians) {
	_yaw = angleRadians;
}

void TrackballCamera::setTarget(const glm::vec3& target) {
	_target = target;
}

void TrackballCamera::setPosition(const glm::vec3& position) {
	_radius = glm::distance(position, _target);
	assert(_radius > 0.0f);

	const glm::vec3 relPosition = position - _target;

	_pitch = glm::asin(-relPosition.y / _radius);
	const float cosPitch = glm::cos(_pitch);
	assert(cosPitch > 0.0f);

	_yaw = glm::asin(relPosition.x / (_radius * cosPitch));

	if (relPosition.z < 0.0f) {
		_yaw = glm::half_pi<float>() + (glm::half_pi<float>() - _yaw);
	}
}

glm::vec3 TrackballCamera::getDirection(const glm::vec3& position) const {
	return glm::normalize(getTarget() - position);
}

glm::vec3 TrackballCamera::getRight() const {
	return glm::vec3(
		glm::sin(_yaw - glm::half_pi<float>()), 0.0f, glm::cos(_yaw - glm::half_pi<float>())
	);
}

glm::vec3 TrackballCamera::getUp() const {
	return glm::cross(getRight(), getDirection());
}

glm::vec3 TrackballCamera::getPosition() const {
	return _target + glm::vec3( // spherical coordinates to cartesian coordinates conversion
		_radius * glm::cos(_pitch) * glm::sin(_yaw),
		-_radius * glm::sin(_pitch),
		_radius * glm::cos(_pitch) * glm::cos(_yaw)
	);
}

glm::mat4 TrackballCamera::getViewMatrix(const glm::vec3& position) const {
	return glm::lookAt(position, _target, glm::vec3(0, 1, 0));
}

}