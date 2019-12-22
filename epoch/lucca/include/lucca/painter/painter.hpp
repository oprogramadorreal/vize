#ifndef LUCCA_PAINTER_HPP
#define LUCCA_PAINTER_HPP

#include "lucca/config.hpp"

#include <boost/noncopyable.hpp>

namespace ayla {
	class AxisAlignedBox;
}

namespace lucca {

class RenderTarget;
class Camera;

/**
 * Knows how to draw something on a render target.
 *
 * @see lucca::RenderTarget
 *
 * @author O Programador
 */
class LUCCA_API Painter : boost::noncopyable {
public:
	virtual ~Painter() = default;

	/**
	 * Initialize this painter with some render target. Must be called before drawing.
	 */
	virtual void initialize(RenderTarget* target) = 0;

	/**
	 * Release resources and undo the job made on "initialize" method.
	 */
	virtual void dispose() = 0;

	/**
	 * Draw data from @param camera point of view.
	 *
	 * @param camera must be associated with the same
	 * render target that was used to initialize this painter.
	 *
	 * @return True on success. False to indicate that it needs repaint.
	 */
	virtual bool paint(const Camera& camera) = 0;

	/**
	 * Tells this painter that the render target was resized.
	 */
	virtual void resize(SizeType width, SizeType height) = 0;

	/**
	 * @return True if the render target used to initialize
	 * this painter is the same object of @param target.
	 */
	virtual bool compareRenderTarget(const RenderTarget* target) const = 0;

	/**
	 * @return True if the render target of this painter has the current rendering context.
	 */
	virtual bool hasCurrentRenderingContext() const = 0;

	/**
	 * @return A bounding box for the data rendered by this painter.
	 */
	virtual ayla::AxisAlignedBox getAABB() const = 0;

	/**
	 * @return A copy of this painter.
	 */
	virtual std::unique_ptr<Painter> clone() const = 0;

	/**
	 * @see lucca::PainterComposite
	 */
	virtual void addSubPainter(std::unique_ptr<Painter> subPainter) = 0;
};

}

#endif // LUCCA_PAINTER_HPP