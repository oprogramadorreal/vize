#ifndef AYLA_GL_TEXT_RENDERER_HPP
#define AYLA_GL_TEXT_RENDERER_HPP

#include "ayla_gl/config.hpp"

#include <boost/noncopyable.hpp>
#include <string>

namespace ayla_gl {

/**
 * It knows how to draw 2D text on the screen.
 */
class AYLA_GL_API TextRenderer final : boost::noncopyable {
public:
	TextRenderer(const std::string& fontTextureFile);
	~TextRenderer();

public:
	void drawText2D(const std::string& text, float x, float y, float size) const;

private:
	GLuint _vao = GLuint(0); // vertex array object
	GLuint _texture = GLuint(0);
	GLuint _vertexBuffer = GLuint(0);
	GLuint _uvBuffer = GLuint(0);
	GLuint _program = GLuint(0);
	GLuint _uniform = GLuint(0);
};

}

#endif // AYLA_GL_TEXT_RENDERER_HPP