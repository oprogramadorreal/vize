#include <GL/glew.h>

#include "vize/opengl/gl_framebuffer_texture_2d.hpp"

namespace vize {

GLFramebufferTexture2D::GLFramebufferTexture2D(SizeType textureWidth, SizeType textureHeight) {
	// create texture:
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	resizeTexture(textureWidth, textureHeight);
	
	// create framebuffer:
	glGenFramebuffers(1, &_framebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _textureID, 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

GLFramebufferTexture2D::~GLFramebufferTexture2D() {
	glDeleteFramebuffers(1, &_framebufferID);
	glDeleteTextures(1, &_textureID);
}

void GLFramebufferTexture2D::resizeTexture(SizeType textureWidth, SizeType textureHeight) {
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, textureWidth, textureHeight, 0, GL_RGB, GL_FLOAT, 0);
}

}