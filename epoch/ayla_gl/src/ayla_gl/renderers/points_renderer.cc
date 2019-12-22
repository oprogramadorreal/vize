#include <GL/glew.h>

#include "ayla_gl/renderers/points_renderer.hpp"

namespace ayla_gl {

PointsRenderer::PointsRenderer(const std::shared_ptr<SimpleProgram>& shader, const std::vector<glm::vec3>& points)
	: _shader(shader), _points(points)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

PointsRenderer::~PointsRenderer() {
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void PointsRenderer::_updateVBO() const {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, getNumberOfPoints() * sizeof(glm::vec3), (void*)_points.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	_needsUpdate = false;
}

void PointsRenderer::_drawBegin(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
	if (_needsUpdate) {
		_updateVBO();
	}

	glUseProgram(_shader->getId());

	const glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(_shader->getUniform_M(), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(_shader->getUniform_V(), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(_shader->getUniform_P(), 1, GL_FALSE, &projectionMatrix[0][0]);

	glBindVertexArray(_vao);
}

void PointsRenderer::_drawEnd() const {
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointsRenderer::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, SizeType numberOfPoints) const {
	_drawBegin(viewMatrix, projectionMatrix);
	glDrawArrays(GL_POINTS, 0, GLsizei(glm::min(numberOfPoints, SizeType(_points.size()))));
	_drawEnd();
}

void PointsRenderer::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
	draw(viewMatrix, projectionMatrix, SizeType(_points.size()));
}

void PointsRenderer::drawLineStrip(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
	_drawBegin(viewMatrix, projectionMatrix);
	glDrawArrays(GL_LINE_STRIP, 0, GLsizei(_points.size()));
	_drawEnd();
}

}