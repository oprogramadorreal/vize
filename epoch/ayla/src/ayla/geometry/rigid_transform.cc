#include "ayla/geometry/rigid_transform.hpp"
#include "ayla/geometry/vector.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace ayla {

void RigidTransform::normalizeOrientation() {
	_r = glm::normalize(_r);
}

void RigidTransform::rotateLocal(Float angleRadians, const glm::vec3& axisLocal) {
	const glm::quat rot = glm::angleAxis(angleRadians, axisLocal);

	_r = _r * rot;

	normalizeOrientation();
}

void RigidTransform::setIdentity() {
	_r = glm::quat();
	_t = glm::vec3(0,0,0);
}

glm::vec3 RigidTransform::transformInverseDirection(const glm::vec3& dir) const {
	return glm::inverse(_r) * dir;
}

RigidTransform RigidTransform::getInverse() const {
	const glm::quat inv = glm::inverse(_r);

	return RigidTransform(
		inv * -_t, inv
	);
}

glm::mat4 RigidTransform::toMatrix4() const {
	//return glm::translate(glm::toMat4(_r), _t); // translate first, then rotate relative to the translation.
	return glm::translate(glm::mat4(1.0f), _t) * glm::toMat4(_r); // rotate first, then translate.
}

std::ostream& operator<<(std::ostream& ostream, const RigidTransform& a) {
	ostream << "RigidTransform(" << a._t << ", glm::quat(" << a._r.x << ", " << a._r.y << ", " << a._r.z << ", " << a._r.w << "))";
	return ostream;
}

}
