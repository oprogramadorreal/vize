#include <GL/glew.h>

#include "ayla_gl/renderers/triangle_mesh_renderer.hpp"

namespace ayla_gl {

TriangleMeshRenderer::TriangleMeshRenderer(const ayla::TriangleMesh& mesh, GLenum vboUsage)
	: _mesh(mesh)
{
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.getNumberOfFaces() * sizeof(ayla::TriangularFace), _mesh.getFaces().data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _mesh.getNumberOfVertices() * sizeof(ayla::Vertex), _mesh.getVertices().data(), vboUsage);

	// vertex
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ayla::Vertex), (void*)0);

	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ayla::Vertex), (void*) sizeof(glm::vec3));

	// uvs
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ayla::Vertex), (void*)(sizeof(glm::vec3) * 2));

	// unbind buffers and vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TriangleMeshRenderer::~TriangleMeshRenderer() {
	glDeleteBuffers(1, &_ibo);
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void TriangleMeshRenderer::updateVertexBufferData() const {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _mesh.getNumberOfVertices() * sizeof(ayla::Vertex), _mesh.getVertices().data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TriangleMeshRenderer::updateIndexBufferData() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh.getNumberOfFaces() * sizeof(ayla::TriangularFace), _mesh.getFaces().data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriangleMeshRenderer::drawElements() const {
	glBindVertexArray(_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glDrawElements(GL_TRIANGLES, GLsizei(_mesh.getNumberOfFaces() * 3u), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

}