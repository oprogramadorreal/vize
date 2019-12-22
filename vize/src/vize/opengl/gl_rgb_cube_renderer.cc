#include <GL/glew.h>

#include "vize/opengl/gl_rgb_cube_renderer.hpp"
#include "vize/shader/colored_vertex_program.hpp"

namespace vize {

/*
 *         green = (2)________________(6)
 *                 |\               |\
 *                 | \              | \
 *                 |  \             |  \
 *                 |(3)\________________\(7) = white
 *                 |   |            |   |
 *      black = (0)|_ _|_ _ _ _ _ _ |(4)| = red
 *                  \  |             \  |
 *                   \ |              \ |
 *                    \|_______________\|
 *             blue = (1)               (5)
 *    |g+
 *    |
 *    |_____r+
 *     \
 *      \b+
 */
namespace {

static const GLfloat _vertexData[] = {
	-0.5f, -0.5f, -0.5f, // 0
	-0.5f, -0.5f,  0.5f, // 1
	-0.5f,  0.5f, -0.5f, // 2
	-0.5f,  0.5f,  0.5f, // 3
	0.5f, -0.5f, -0.5f, // 4
	0.5f, -0.5f,  0.5f, // 5
	0.5f,  0.5f, -0.5f, // 6
	0.5f,  0.5f,  0.5f  // 7
};

static const GLfloat _colorData[] = {
	0.0f, 0.0f, 0.0f, // 0
	0.0f, 0.0f, 1.0f, // 1
	0.0f, 1.0f, 0.0f, // 2
	0.0f, 1.0f, 1.0f, // 3
	1.0f, 0.0f, 0.0f, // 4
	1.0f, 0.0f, 1.0f, // 5
	1.0f, 1.0f, 0.0f, // 6
	1.0f, 1.0f, 1.0f  // 7
};

static const unsigned short _indexData[] = {
	0, 1, 3,
	0, 3, 2,
	1, 5, 7,
	1, 7, 3,
	5, 4, 6,
	5, 6, 7,
	4, 0, 6,
	0, 2, 6,
	3, 7, 6,
	3, 6, 2,
	0, 4, 5,
	0, 5, 1
};

}

GLRGBCubeRenderer::GLRGBCubeRenderer() {
	_shader = std::make_unique<ColoredVertexProgram>();

	glGenBuffers(3, _buffersIDs);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertexData), _vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_colorData), _colorData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indexData), _indexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
}

GLRGBCubeRenderer::~GLRGBCubeRenderer() {
	glDeleteBuffers(3, _buffersIDs);
	_shader.reset();
}

void GLRGBCubeRenderer::draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix) const {
	glUseProgram(_shader->getId());

	glUniformMatrix4fv(_shader->getUniform_M(), 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(_shader->getUniform_V(), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(_shader->getUniform_P(), 1, GL_FALSE, &projectionMatrix[0][0]);

	// 1st attribute: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute: colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glDrawElements(GL_TRIANGLES, sizeof(_indexData) / sizeof(unsigned short), GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
	glUseProgram(0);
}

}