#include <GL/glew.h>

#include "vize/opengl/gl_volume_texture.hpp"
#include "vize/model/volume.hpp"

#include <ayla_gl/print_opengl_error.hpp>

namespace vize {

GLVolumeTexture::GLVolumeTexture(const Volume& volume) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_3D, _id);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);

	// see: http://stackoverflow.com/questions/11042027/glpixelstoreigl-unpack-alignment-1-disadvantages
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// see: https://www.khronos.org/opengles/sdk/docs/man3/html/glTexImage3D.xhtml
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R8, volume.getWidth(), volume.getHeight(), volume.getDepth(), 0, GL_RED, GL_UNSIGNED_BYTE, (void*)volume.getRawData());

	if (ayla_gl::checkOpenGLError()) {
		glDeleteTextures(1, &_id);
		_id = GLuint(0);
	}
}

GLVolumeTexture::~GLVolumeTexture() {
	glDeleteTextures(1, &_id);
}

}