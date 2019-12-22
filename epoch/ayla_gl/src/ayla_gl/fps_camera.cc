#include "ayla_gl/fps_camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace ayla_gl {

FPSCamera::FPSCamera(const glm::vec3& position)
	: _position(position), _pitch(0.0f), _yaw(glm::pi<float>())
{ }

void FPSCamera::pitch(float angleRadians) {
	_pitch += angleRadians;
}

void FPSCamera::yaw(float angleRadians) {
	_yaw += angleRadians;
}

void FPSCamera::translate(const glm::vec3& amount) {
	_position += amount;
}

void FPSCamera::setPosition(const glm::vec3& position) {
	_position = position;
}

glm::vec3 FPSCamera::getDirection() const {
	return glm::vec3 ( // spherical coordinates to cartesian coordinates conversion
		glm::cos(_pitch) * glm::sin(_yaw),
		glm::sin(_pitch),
		glm::cos(_pitch) * glm::cos(_yaw)
	);
}

glm::vec3 FPSCamera::getRight() const {
	return glm::vec3 (
		glm::sin(_yaw - glm::half_pi<float>()), 0.0f, glm::cos(_yaw - glm::half_pi<float>())
	);
}

glm::vec3 FPSCamera::getUp() const {
	return glm::cross(getRight(), getDirection());
}

glm::mat4 FPSCamera::getViewMatrix() const {
	const glm::vec3 f = getDirection();
	const glm::vec3 r = getRight();
	const glm::vec3 u = glm::cross(r, f); // up

	return glm::mat4(
		r.x, u.x, -f.x, 0,
		r.y, u.y, -f.y, 0,
		r.z, u.z, -f.z, 0,
		-glm::dot(r, _position), -glm::dot(u, _position), glm::dot(f, _position), 1.0f
	);
}

}