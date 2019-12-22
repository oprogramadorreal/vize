#ifndef LUCCA_RENDER_TARGET_HPP
#define LUCCA_RENDER_TARGET_HPP

#include "lucca/config.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

/**
 * @see lucca::View
 *
 * @author O Programador
 */
class LUCCA_API RenderTarget : boost::noncopyable {
public:
	virtual ~RenderTarget() = default;

	virtual bool hasCurrentRenderingContext() const = 0;

	virtual void makeCurrentRenderingContext() = 0;
	virtual void doneCurrentRenderingContext() = 0;

	virtual SizeType getDefaultFramebufferObject() const = 0;

	virtual SizeType getRenderTargetWidth() const = 0;
	virtual SizeType getRenderTargetHeight() const = 0;

	virtual void postRedisplay() = 0;
};

}

#endif // LUCCA_RENDER_TARGET_HPP