#ifndef LUCCA_VIEW_COMPOSITE_HPP
#define LUCCA_VIEW_COMPOSITE_HPP

#include "lucca/config.hpp"

#include "lucca/view/view.hpp"
#include "lucca/view/view_signals.hpp"

namespace lucca {

/**
 * Implements lucca::View interface based on the "composite pattern".
 * This forwards all calls to the active view.
 *
 * @see lucca::View
 * @see https://en.wikipedia.org/wiki/Composite_pattern
 *
 * @author O Programador
 */
class LUCCA_API ViewComposite : public View {
public:
	virtual ~ViewComposite();

public:
	virtual bool addSubView(std::unique_ptr<View> view) = 0;

	virtual void removeAllSubViews() = 0;

	virtual SizeType getNumberOfSubViews() const = 0;

	virtual void visitSubViews(const std::function<void(View&)>&) const = 0;

	virtual View* getActiveSubView() const = 0;

public:
	using SubWindowTitleGenerator = std::function<std::string()>;

	virtual void setSubWindowTitleGenerator(const SubWindowTitleGenerator& titleGenerator) = 0;

	virtual SubWindowTitleGenerator getSubWindowTitleGenerator() const = 0;

	virtual boost::signals2::connection connectToSubViewActivatedSignal(const SubViewActivatedSignalListener& listener) = 0;

private: // lucca::View interface
	virtual void setPainter(std::unique_ptr<Painter> painter) final override;

	virtual Painter* getPainter() const final override;

	virtual Camera* getDefaultCamera() const final override;

	virtual void addCamera(std::unique_ptr<Camera> camera) final override;

	virtual void clearConnections(SizeType connectionsGroup) final override;

	virtual bool connectEventHandler(EventHandler& handler, SizeType connectionsGroup) final override;

	virtual bool setEventDispatcher(const std::shared_ptr<EventDispatcher>& dispatcher) final override;

	virtual ViewComposite* getComposite() final override;

private: // lucca::RenderTarget interface
	virtual bool hasCurrentRenderingContext() const final override;

	virtual void makeCurrentRenderingContext() final override;
	virtual void doneCurrentRenderingContext() final override;

	virtual SizeType getDefaultFramebufferObject() const final override;

	virtual SizeType getRenderTargetWidth() const final override;
	virtual SizeType getRenderTargetHeight() const final override;

	virtual void postRedisplay() final override;
};

}

#endif // LUCCA_VIEW_COMPOSITE_HPP