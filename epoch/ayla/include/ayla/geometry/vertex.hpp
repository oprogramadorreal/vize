#ifndef AYLA_VERTEX_HPP
#define AYLA_VERTEX_HPP

#include "ayla/config.hpp"

#include "ayla/serialization/vertex_serializer.hpp"

namespace ayla {

/**
 * Basic vertex abstraction.
 * Holds position, normal and UV.
 *
 * @see ayla::TriangleMesh
 *
 * @author O Programador
 */
class AYLA_API Vertex final {
public:
	Vertex();
	Vertex(const glm::vec3& position, const glm::vec3& normal = glm::vec3(0.0f), const glm::vec2& uv = glm::vec2(0.0f));

	inline const glm::vec3& getPosition() const { return _position; }
	inline const glm::vec3& getNormal() const { return _normal; }
	inline const glm::vec2& getUV() const { return _uv; }

	inline void setPosition(const glm::vec3& p) { _position = p; }
	inline void setNormal(const glm::vec3& n) { _normal = n; }
	inline void setUV(const glm::vec2& uv) { _uv = uv; }

private:
	glm::vec3 _position = glm::vec3(0.0f);
	glm::vec3 _normal = glm::vec3(0.0f);
	glm::vec2 _uv = glm::vec2(0.0f);

	template<class Archive> friend void boost::serialization::serialize(Archive&, ayla::Vertex&, const unsigned int);
};

}

#endif // AYLA_VERTEX_HPP