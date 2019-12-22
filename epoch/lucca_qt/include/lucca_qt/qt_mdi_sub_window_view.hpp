#ifndef LUCCA_QT_MDI_SUB_WINDOW_VIEW_HPP
#define LUCCA_QT_MDI_SUB_WINDOW_VIEW_HPP

#include "lucca_qt/config.hpp"

#include <lucca/view/view.hpp>

#include <QMdiSubWindow>

namespace lucca_qt {

class QtOpenGLView;

/**
 * This forwards all calls to the provided lucca::QtOpenGLView instance.
 * To be used in conjunction with lucca::QtOpenGLView and lucca::QtMdiAreaView.
 *
 * @remarks This is an abstract class. lucca::View::clone() method is not implemented.
 *
 * @see lucca::QtMdiAreaView
 * @see lucca::QtOpenGLView
 *
 * @author O Programador
 */
class LUCCA_QT_API QtMdiSubWindowView : public QMdiSubWindow, public lucca::View {
public:
	QtMdiSubWindowView(QtOpenGLView& widget);
	virtual ~QtMdiSubWindowView();

public:
	View& getWidgetView() const;

	void setWidgetViewCursor(const QCursor& cursor);

public: // View interface
	virtual void setPainter(std::unique_ptr<lucca::Painter> painter) final override;
	virtual lucca::Painter* getPainter() const final override;
	virtual lucca::Camera* getDefaultCamera() const final override;
	virtual void addCamera(std::unique_ptr<lucca::Camera> camera) final override;
	virtual void clearConnections(SizeType connectionsGroup) final override;
	virtual bool connectEventHandler(lucca::EventHandler& handler, SizeType connectionsGroup) final override;
	virtual bool setEventDispatcher(const std::shared_ptr<lucca::EventDispatcher>& dispatcher) final override;
	virtual lucca::ViewComposite* getComposite() final override;

protected: // RenderTarget interface
	virtual bool hasCurrentRenderingContext() const final override;
	virtual void makeCurrentRenderingContext() final override;
	virtual void doneCurrentRenderingContext() final override;
	virtual SizeType getDefaultFramebufferObject() const final override;
	virtual SizeType getRenderTargetWidth() const final override;
	virtual SizeType getRenderTargetHeight() const final override;
	virtual void postRedisplay() final override;

private:
	QtOpenGLView& _widgetView;
};

}

#endif // LUCCA_QT_MDI_SUB_WINDOW_VIEW_HPP