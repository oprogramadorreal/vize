#include "ayla/geometry/quaternion.hpp"

namespace std {

// operator<< for glm::quat is defined inside std namespace to make boost tests compilable:
// http://stackoverflow.com/questions/10976130/boost-check-equal-with-pairint-int-and-custom-operator
ostream& operator<<(ostream& stream, const glm::quat& a) {
	stream << "glm::quat(" << a.w << "," << a.x << "," << a.y << "," << a.z << ")";
	return stream;
}

}