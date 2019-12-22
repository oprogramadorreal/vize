#include <GL/glew.h>

#include "ayla_gl/renderers/lines_renderer.hpp"

namespace ayla_gl {

LinesRenderer::LinesRenderer(const std::shared_ptr<SimpleProgram>& shader, const std::vector<ayla::LineSegment>& lines)
	: _shader(shader), _lines(lines)
{
	assert(sizeof(ayla::LineSegment) == 2u * sizeof(glm::vec3));

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

LinesRenderer::~LinesRenderer() {
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void LinesRenderer::_updateVBO() const {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, getNumberOfLines() * sizeof(ayla::LineSegment), (void*)_lines.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	_needsUpdate = false;
}

void LinesRenderer::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const {
	if (_needsUpdate) {
		_updateVBO();
	}
	
	glPushAttrib( GL_ENABLE_BIT | GL_LINE_BIT );
	if( _dashed ) {
		glLineStipple(1, 0x00FF);
		glEnable(GL_LINE_STIPPLE);
	}
	
	glUseProgram(_shader->getId());

	const glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(_shader->getUniform_M(), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(_shader->getUniform_V(), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(_shader->getUniform_P(), 1, GL_FALSE, &projectionMatrix[0][0]);

	glBindVertexArray(_vao);
	glDrawArrays(GL_LINES, 0, GLsizei(_lines.size() * 2u));
	glBindVertexArray(0);

	glUseProgram(0);
	glPopAttrib();
}

}