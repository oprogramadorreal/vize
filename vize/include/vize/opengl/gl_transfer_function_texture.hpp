#ifndef VIZE_GL_TRANSFER_FUNCTION_TEXTURE_HPP
#define VIZE_GL_TRANSFER_FUNCTION_TEXTURE_HPP

#include "vize/config.hpp"

#include <boost/noncopyable.hpp>

namespace vize {

/**
 * @author O Programador
 */
class GLTransferFunctionTexture final : boost::noncopyable {
public:
	GLTransferFunctionTexture();
	~GLTransferFunctionTexture();

public:
	inline GLuint getID() const { return _id; }
	
private:
	GLuint _id = 0u;
};

}

#endif // VIZE_GL_TRANSFER_FUNCTION_TEXTURE_HPP