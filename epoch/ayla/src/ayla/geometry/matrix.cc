#include "ayla/geometry/matrix.hpp"

#include <glm/gtc/epsilon.hpp>

namespace ayla {

bool epsilonEqual(const glm::mat4& a, const glm::mat4& b, Float epsilon) {
	for (SizeType i = 0u; i < 4u; ++i) {
		for (SizeType j = 0u; j < 4u; ++j) {
			if (glm::epsilonNotEqual(a[i][j], b[i][j], epsilon)) {
				return false;
			}
		}

	}

	return true;
}

bool epsilonEqual(const glm::mat3& a, const glm::mat3& b, Float epsilon) {
	for (SizeType i = 0u; i < 3u; ++i) {
		for (SizeType j = 0u; j < 3u; ++j) {
			if (glm::epsilonNotEqual(a[i][j], b[i][j], epsilon)) {
				return false;
			}
		}
	}

	return true;
}

}

namespace std {

ostream& operator<<(ostream& stream, const glm::mat4& a) {
	const auto oldWidth = stream.width();

	stream.width(10);
	stream << "glm::mat4:\n";

	for (ayla::SizeType row = 0u; row < 4u; ++row) {
		stream << "  ";

		for (ayla::SizeType col = 0u; col < 4u; ++col) {
			stream << a[col][row] << " ";
		}

		stream << std::endl;
	}

	stream.width(oldWidth);

	return stream;
}

ostream& operator<<(ostream& stream, const glm::mat3& a) {
	const auto oldWidth = stream.width();

	stream.width(10);
	stream << "glm::mat3:\n";

	for (ayla::SizeType row = 0u; row < 3u; ++row) {
		stream << "  ";

		for (ayla::SizeType col = 0u; col < 3u; ++col) {
			stream << a[col][row] << " ";
		}

		stream << std::endl;
	}

	stream.width(oldWidth);

	return stream;
}

}