#ifndef AYLA_GL_TEXTURE_2D_HPP
#define AYLA_GL_TEXTURE_2D_HPP

#include "ayla_gl/config.hpp"

#include <boost/noncopyable.hpp>

namespace ayla {
	class Image;
}

namespace ayla_gl {

/**
 * @see ayla::Image
 *
 * @author O Programador
 */
class AYLA_GL_API Texture2D final : boost::noncopyable {
public:
	Texture2D(const ayla::Image& image, bool repeat = true);
	~Texture2D();

	void bind() const;

private:
	struct Private;
	std::unique_ptr<Private> _pimpl;
};

}

#endif // AYLA_GL_TEXTURE_2D_HPP