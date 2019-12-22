#ifndef VIZE_GL_FRAMEBUFFER_TEXTURE_2D_HPP
#define VIZE_GL_FRAMEBUFFER_TEXTURE_2D_HPP

#include "vize/config.hpp"

#include <boost/noncopyable.hpp>

namespace vize {

/**
 * @author O Programador
 */
class GLFramebufferTexture2D final : boost::noncopyable {
public:
	GLFramebufferTexture2D(SizeType textureWidth, SizeType textureHeight);
	~GLFramebufferTexture2D();

public:
	void resizeTexture(SizeType textureWidth, SizeType textureHeight);

	inline GLuint getFramebufferID() const { return _framebufferID; }
	inline GLuint getTextureID() const { return _textureID; }
 
private:
	GLuint _framebufferID = GLuint(0);
	GLuint _textureID = GLuint(0);
};

}

#endif // VIZE_GL_FRAMEBUFFER_TEXTURE_2D_HPP