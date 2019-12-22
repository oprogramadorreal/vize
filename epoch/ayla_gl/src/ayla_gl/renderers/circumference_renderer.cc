#include <GL/glew.h>

#include "ayla_gl/renderers/circumference_renderer.hpp"

#include <ayla/geometry/vector.hpp>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <vector>

namespace ayla_gl {

CircumferenceRenderer::CircumferenceRenderer(const std::shared_ptr<SimpleProgram>& shader, const ayla::Circle& circle, SizeType numberOfVertices)
	: _circle(circle), _numberOfVertices(numberOfVertices), _shader(shader)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_generatePoints();
}

CircumferenceRenderer::~CircumferenceRenderer() {
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void CircumferenceRenderer::_generatePoints() {
	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	std::vector<glm::vec3> points;
	const Float stepAngle = (1.0f / Float(_numberOfVertices)) * (2.0f * glm::pi<Float>());

	for (SizeType i = 0u; i < _numberOfVertices; ++i) {
		points.push_back(_circle.getPointOnCircumference(i * stepAngle));
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CircumferenceRenderer::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
	glUseProgram(_shader->getId());

	glUniformMatrix4fv(_shader->getUniform_V(), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(_shader->getUniform_P(), 1, GL_FALSE, &projectionMatrix[0][0]);

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINE_LOOP, 0, GLsizei(_numberOfVertices));
	glBindVertexArray(0);

	glUseProgram(0);
}

}