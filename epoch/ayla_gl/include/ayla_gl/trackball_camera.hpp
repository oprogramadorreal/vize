#ifndef AYLA_GL_TRACKBALL_CAMERA_HPP
#define AYLA_GL_TRACKBALL_CAMERA_HPP

#include "ayla_gl/config.hpp"

namespace ayla_gl {

/**
 * Make a camera orbit a "virtual sphere" centered at a chosen target point.
 *
 * @author O Programador
 */
class AYLA_GL_API TrackballCamera final {
public:
	TrackballCamera(const glm::vec3& target, float radius);

	void addPitch(float angleRadians);
	void addYaw(float angleRadians);
	void setPitch(float angleRadians);
	void setYaw(float angleRadians);

	inline float getPitch() const { return _pitch; }
	inline float getYaw() const { return _yaw; }

	void setTarget(const glm::vec3& target);
	inline const glm::vec3& getTarget() const { return _target; }
	
	inline void setRadius(float radius) { _radius = radius; }
	inline float getRadius() const { return _radius; }

	inline glm::vec3 getDirection() const { return getDirection(getPosition()); }
	glm::vec3 getDirection(const glm::vec3& position) const;

	glm::vec3 getRight() const;

	/**
	 * @warning The returned vector might not be normalized.
	 */
	glm::vec3 getUp() const;

	glm::vec3 getPosition() const;

	void setPosition(const glm::vec3& position);
	
	inline glm::mat4 getViewMatrix() const {
		return getViewMatrix(getPosition());
	}
	glm::mat4 getViewMatrix(const glm::vec3& position) const;

private:
	glm::vec3 _target = glm::vec3(0.0f);
	float _radius = 0.0f;
	float _pitch = 0.0f;
	float _yaw = 0.0f;
};


}

#endif // AYLA_GL_TRACKBALL_CAMERA_HPP