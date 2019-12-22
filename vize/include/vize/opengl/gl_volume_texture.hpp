#ifndef VIZE_GL_VOLUME_TEXTURE_HPP
#define VIZE_GL_VOLUME_TEXTURE_HPP

#include "vize/config.hpp"

#include <boost/noncopyable.hpp>

namespace vize {

class Volume;

/**
 * Loads an OpenGL 3D texture from a vize::Volume.
 *
 * @see vize::Volume
 *
 * @author O Programador
 */
class GLVolumeTexture final : boost::noncopyable {
public:
	GLVolumeTexture(const Volume& volume);
	~GLVolumeTexture();

public:
	inline GLuint getID() const { return _id; }
	
private:
	GLuint _id = 0u;
};

}

#endif // VIZE_GL_VOLUME_TEXTURE_HPP