#include "ayla/geometry/vertex.hpp"

namespace ayla {

Vertex::Vertex() = default;

Vertex::Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv)
	: _position(position), _normal(normal), _uv(uv)
{ }

}