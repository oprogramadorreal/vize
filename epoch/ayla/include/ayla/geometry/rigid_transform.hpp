#ifndef AYLA_RIGID_TRANSFORM_HPP
#define AYLA_RIGID_TRANSFORM_HPP

#include "ayla/config.hpp"

#include <glm/gtx/quaternion.hpp>

namespace ayla {

/**
 * A class to represents rigid transformations (also known as "rigid body transformations").
 * 
 * @remarks Rigid transformations are composed only by translations and rotations. There is no scale transformations.
 * Internally, it keeps a glm::quat (or a glm::mat3) to represent an orientation and a glm::vec3 to represent a translation.
 * The inverse of a rigid transform is very easy and fast to calculate, what is not true for a full transformation involving also scales and shear.
 *
 * @see http://en.wikipedia.org/wiki/Rigid_transformation
 *
 * @author O Programador
 */
class AYLA_API RigidTransform final {
public:
	inline static RigidTransform getIdentity() {
		return RigidTransform(glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
	}

public:
	RigidTransform() = default;

	inline explicit RigidTransform(const glm::vec3& translation, const glm::mat3& rotation = glm::mat3(1.0f)) {
		set(translation, rotation);
	}

	inline RigidTransform(const glm::vec3& translation, const glm::quat& rotation) {
		set(translation, rotation);
	}

	inline RigidTransform(const glm::quat& rotation) {
		set(glm::vec3(0,0,0), rotation);
	}

	inline void set(const glm::vec3& t, const glm::mat3& r) {
		setTranslation(t);
		setRotation(r);
	}

	inline void set(const glm::vec3& t, const glm::quat& r) {
		setTranslation(t);
		setRotation(r);
	}

	inline void setTranslation(const glm::vec3& t) {
		_t = t;
	}

	inline void setRotation(const glm::mat3& r) {
		_r = glm::quat_cast(r);
	}

	inline void setRotation(const glm::quat& r) {
		_r = r;
	}

	void setIdentity();

	inline glm::vec3 getTranslation() const {
		return _t;
	}

	inline glm::mat3 getRotationMatrix() const {
		return glm::mat3_cast(_r);
	}

	inline const glm::quat& getRotation() const {
		return _r;
	}

	inline void translate(const glm::vec3& tWorld) {
		_t += tWorld;
	}

	void rotateLocal(Float angleRadians, const glm::vec3& axisLocal);

	inline void rotate(Float angleRadians, const glm::vec3& axisWorld) {
		rotateLocal(angleRadians, transformInverseDirection(axisWorld));
	}

	inline glm::vec3 transform(const glm::vec3& point) const {
		return (_r * point) + _t;
	}

	inline glm::vec3 transformInverse(const glm::vec3& point) const {
		return getInverse().transform(point);
	}

	inline glm::vec3 transformDirection(const glm::vec3& dir) const {
		return _r * dir;
	}

	glm::vec3 transformInverseDirection(const glm::vec3& dir) const;

	RigidTransform getInverse() const;

	inline glm::vec3 invXform(const glm::vec3& inVec) const {
		return transformInverseDirection( glm::vec3(inVec - _t) );
	}

	inline RigidTransform operator*(const RigidTransform& other) const {
		return RigidTransform(
			transform(other._t), _r*other._r
		);
	}

	inline void operator*=(const RigidTransform& other) {
		*this = RigidTransform(*this) * other;
	}

	inline bool operator==(const RigidTransform& other) const {
		return (_r == other._r) && (_t == other._t);
	}

	void normalizeOrientation();

	inline glm::vec3 getPointInLocalSpace(const glm::vec3 &pt) const { return transformInverse(pt); }
	inline glm::vec3 getPointInWorldSpace(const glm::vec3 &pt) const { return transform(pt); }
	inline glm::vec3 getDirectionInLocalSpace(const glm::vec3 &dir) const { return transformInverseDirection(dir); }
	inline glm::vec3 getDirectionInWorldSpace(const glm::vec3 &dir) const { return transformDirection(dir); }

	glm::mat4 toMatrix4() const;

	friend std::ostream& operator<<(std::ostream& ostream, const RigidTransform& a);

private:
	glm::vec3 _t = glm::vec3(0.0f);
	glm::quat _r = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

}

#endif // AYLA_RIGID_TRANSFORM_HPP
