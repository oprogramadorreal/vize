#ifndef VIZE_GL_RGB_CUBE_RENDERER_HPP
#define VIZE_GL_RGB_CUBE_RENDERER_HPP

#include "vize/config.hpp"

#include <boost/noncopyable.hpp>

namespace vize {

class ColoredVertexProgram;

/**
 * Knows how to draw a RGB cube.
 *
 * @author O Programador
 */
class GLRGBCubeRenderer final : boost::noncopyable {
public:
	GLRGBCubeRenderer();
	~GLRGBCubeRenderer();

public:
	void draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::mat4& modelMatrix = glm::mat4(1.0f)) const;

private:
	std::unique_ptr<ColoredVertexProgram> _shader;

	union {
		struct {
			GLuint _vertexBuffer;
			GLuint _colorBuffer;
			GLuint _indexBuffer;
		};

		GLuint _buffersIDs[3];
	};
};

}

#endif // VIZE_GL_RGB_CUBE_RENDERER_HPP