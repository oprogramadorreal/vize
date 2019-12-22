#ifndef AYLA_GL_FPS_CAMERA_HPP
#define AYLA_GL_FPS_CAMERA_HPP

#include "ayla_gl/config.hpp"

namespace ayla_gl {

/**
 * Abstraction to help controlling a camera in a first-person shooter (FPS) game style.
 *
 * @author O Programador
 */
class AYLA_GL_API FPSCamera final {
public:
	FPSCamera(const glm::vec3& position = glm::vec3(0, 0, 0));

	void pitch(float angleRadians);
	void yaw(float angleRadians);

	void translate(const glm::vec3& amount);

	inline const glm::vec3& getPosition() const { return _position; }
	void setPosition(const glm::vec3& position);

	glm::vec3 getDirection() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;

	glm::mat4 getViewMatrix() const;

private:
	glm::vec3 _position = glm::vec3(0.0f);
	float _pitch = 0.0f;
	float _yaw = 0.0f;
};


}

#endif // AYLA_GL_FPS_CAMERA_HPP