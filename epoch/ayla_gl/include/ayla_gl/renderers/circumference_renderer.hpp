#ifndef AYLA_GL_CIRCUMFERENCE_RENDERER_HPP
#define AYLA_GL_CIRCUMFERENCE_RENDERER_HPP

#include "ayla_gl/config.hpp"

#include "ayla_gl/shaders/simple_program.hpp"
#include <ayla/geometry/circle.hpp>

namespace ayla_gl {

/**
 * Draw a circle in wireframe.
 *
 * @author O Programador
 */
class AYLA_GL_API CircumferenceRenderer final : boost::noncopyable {
public:
	inline CircumferenceRenderer(const ayla::Circle& circle, SizeType numberOfVertices = 100u)
		: CircumferenceRenderer(std::make_shared<SimpleProgram>(), circle, numberOfVertices)
	{ }

	CircumferenceRenderer(const std::shared_ptr<SimpleProgram>& shader, const ayla::Circle& circle, SizeType numberOfVertices = 100u);

	~CircumferenceRenderer();

	inline const ayla::Circle& getCircle() const { return _circle; }

	inline void setCircle(const ayla::Circle& c) {
		_circle = c;
		_generatePoints();
	}

	inline const std::shared_ptr<SimpleProgram>& getShader() const { return _shader; }

	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const;

private:
	void _generatePoints();

private:
	ayla::Circle _circle;
	const SizeType _numberOfVertices;

	GLuint _vao = 0u; // vertex array object
	GLuint _vbo = 0u; // vertex buffer object

	std::shared_ptr<SimpleProgram> _shader;
};

}

#endif // AYLA_GL_CIRCUMFERENCE_RENDERER_HPP