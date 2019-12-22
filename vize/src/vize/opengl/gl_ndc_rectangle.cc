#include <GL/glew.h>

#include "vize/opengl/gl_ndc_rectangle.hpp"

namespace vize {

/*
 * Two triangles to form the rectangle where our raycasting shader will be applied.
 * This rectangle will cover the entire screen.
 *
 *  (3)_______________(2)
 *    |               |
 *    |               |
 *    |               |
 *    |               |
 *    |_______________|
 *  (0)               (1)
 */
namespace {

const glm::vec2 _rectangleVertexData[4] = {
	glm::vec2(-1.0f, -1.0f), // 0
	glm::vec2( 1.0f, -1.0f), // 1
	glm::vec2( 1.0f,  1.0f), // 2
	glm::vec2(-1.0f,  1.0f)  // 3
};

const unsigned short _rectangleIndexData[6] = {
	0u, 1u, 2u,
	0u, 2u, 3u
};

}

GLNDCRectangle::GLNDCRectangle() {
	glGenBuffers(2, _rectangleBuffersIDs);
	glBindBuffer(GL_ARRAY_BUFFER, _rectangleVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 4u * sizeof(glm::vec2), _rectangleVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rectangleIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6u * sizeof(unsigned short), _rectangleIndexData, GL_STATIC_DRAW);
}

GLNDCRectangle::~GLNDCRectangle() {
	glDeleteBuffers(2, _rectangleBuffersIDs);
}

void GLNDCRectangle::drawElements(SizeType shaderAttributeIndex) const {
	glEnableVertexAttribArray(shaderAttributeIndex);
	glBindBuffer(GL_ARRAY_BUFFER, _rectangleVertexBuffer);
	glVertexAttribPointer(shaderAttributeIndex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rectangleIndexBuffer);
	glDrawElements(GL_TRIANGLES, 6u, GL_UNSIGNED_SHORT, (void*)0);

	glDisableVertexAttribArray(shaderAttributeIndex);
}

}