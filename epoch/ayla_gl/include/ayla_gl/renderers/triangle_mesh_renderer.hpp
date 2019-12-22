#ifndef AYLA_GL_TRIANGLE_MESH_RENDERER_HPP
#define AYLA_GL_TRIANGLE_MESH_RENDERER_HPP

#include "ayla_gl/config.hpp"

#include <ayla/geometry/triangle_mesh.hpp>
#include <boost/noncopyable.hpp>

namespace ayla_gl {

/**
 * This creates two OpenGL buffers:
 * - One vertex buffer object (VBO) to keep the vertices.
 * - One index buffer object (IBO) to keep the indices on the triangle mesh faces.
 *
 * Also, this creates one Vertex Array Object (VAO) with 3 vertex attributes enabled:
 * [0] positions (vec3)
 * [1] normals (vec3)
 * [2] UV texture coordinates (vec2)
 *
 * Therefore, to access this data on the vertex shader, for example,
 * declare the shader input variables with something like this:
 *
 * layout(location = 0) in vec3 vertexPosition;
 * layout(location = 1) in vec3 vertexNormal;
 * layout(location = 2) in vec2 vertexUV;
 *
 * @author O Programador
 */
class AYLA_GL_API TriangleMeshRenderer final : boost::noncopyable {
public:
	TriangleMeshRenderer(const ayla::TriangleMesh& mesh, GLenum vboUsage = GL_DYNAMIC_DRAW);

	~TriangleMeshRenderer();

	/**
	 * Resend triangle mesh vertex data to OpenGL.
	 */
	void updateVertexBufferData() const;

	/**
	 * Resend face indices data to OpenGL.
	 */
	void updateIndexBufferData() const;

	/**
	 * Draw the triangle mesh on the current framebuffer.
	 */
	void drawElements() const;
		
	/**
	 * You may need this if you want to add attributes to the Vertex Array Object (VAO).
	 * See the "tension mapping" sample, where this operation is performed.
	 */
	inline GLuint getVertexArrayObject() const { return _vao; }

private:
	const ayla::TriangleMesh& _mesh;
	GLuint _ibo = 0u; // index buffer object
	GLuint _vbo = 0u; // vertex buffer object
	GLuint _vao = 0u; // vertex array object
};

}

#endif // AYLA_GL_TRIANGLE_MESH_RENDERER_HPP