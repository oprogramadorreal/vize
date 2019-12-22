#include "ayla/geometry/vector.hpp"

namespace std {

std::ostream& operator<<(ostream &out, const glm::vec3& v) {
	out << "glm::vec3(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	return out;
}

}

namespace ayla {

glm::vec3 orthogonal(const glm::vec3& r) {
	if (glm::abs(r.x) <= glm::abs(r.y) && glm::abs(r.x) <= glm::abs(r.z)) {
		return glm::vec3(0.0f, -r.z, r.y);
	}

	if (glm::abs(r.y) <= glm::abs(r.x) && glm::abs(r.y) <= glm::abs(r.z)) {
		return glm::vec3(-r.z, 0.0f, r.x);
	}

	return glm::vec3(-r.y, r.x, 0.0f);
}

glm::vec2 transformPosition(const glm::mat3& transform, const glm::vec2& pos) {
	const auto tmp = transform * glm::vec3(pos, 1);
	return glm::vec2(tmp.x / tmp.z, tmp.y / tmp.z);
}

glm::vec3 transformPosition(const glm::mat4& transform, const glm::vec3& pos) {
	const auto tmp = transform * glm::vec4(pos, 1);
	return glm::vec3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);
}

glm::vec2 transformDirection(const glm::mat3& transform, const glm::vec2& pos) {
	const auto tmp = transform * glm::vec3(pos, 0);
	return glm::vec2(tmp.x, tmp.y);
}

glm::vec3 transformDirection(const glm::mat4& transform, const glm::vec3& pos) {
	const auto tmp = transform * glm::vec4(pos, 0);
	return glm::vec3(tmp.x, tmp.y, tmp.z);
}

}