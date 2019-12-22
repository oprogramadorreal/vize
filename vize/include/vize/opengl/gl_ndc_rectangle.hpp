#ifndef VIZE_GL_NDC_RECTANGLE_HPP
#define VIZE_GL_NDC_RECTANGLE_HPP

#include "vize/config.hpp"

#include <boost/noncopyable.hpp>

namespace vize {

/**
 * @author O Programador
 */
class GLNDCRectangle final : boost::noncopyable {
public:
	GLNDCRectangle();
	~GLNDCRectangle();

	void drawElements(SizeType shaderAttributeIndex = SizeType(0)) const;

private:
	union {
		struct {
			GLuint _rectangleVertexBuffer;
			GLuint _rectangleIndexBuffer;
		};

		GLuint _rectangleBuffersIDs[2];
	};
};

}

#endif // VIZE_GL_NDC_RECTANGLE_HPP