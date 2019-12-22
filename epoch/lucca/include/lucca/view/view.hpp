#ifndef LUCCA_VIEW_HPP
#define LUCCA_VIEW_HPP

#include "lucca/config.hpp"

#include "lucca/render_target.hpp"

namespace lucca {

class Painter;
class Camera;
class ViewComposite;
class EventDispatcher;
class EventHandler;

/**
 * Basic viewing interface.
 *
 * A view is a render target that binds
 * a painter to one or more cameras.
 *
 * @see lucca::Painter
 * @see lucca::Camera
 * @see lucca::RenderTarget
 *
 * @author O Programador
 */
class LUCCA_API View : public RenderTarget {
public:
	virtual ~View() = default;

	virtual std::unique_ptr<View> clone() const = 0;

	virtual void setPainter(std::unique_ptr<Painter> painter) = 0;

	virtual Painter* getPainter() const = 0;

	virtual Camera* getDefaultCamera() const = 0;

	virtual void addCamera(std::unique_ptr<Camera> camera) = 0;

	virtual void clearConnections(SizeType connectionsGroup = SizeType(0)) = 0;

	virtual bool connectEventHandler(EventHandler& handler, SizeType connectionsGroup = SizeType(0)) = 0;

	virtual bool setEventDispatcher(const std::shared_ptr<EventDispatcher>& dispatcher) = 0;

	/**
	 * @return The view composite to which this view is attached,
	 * nullptr if it is not attached or itself if this is already a composite view.
	 *
	 * @see lucca::ViewComposite
	 */
	virtual ViewComposite* getComposite() { return nullptr; }
};

}

#endif // LUCCA_VIEW_HPP