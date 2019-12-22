#ifndef LUCCA_QT_OPENGL_VIEW_HPP
#define LUCCA_QT_OPENGL_VIEW_HPP

#include "lucca_qt/config.hpp"

#include <lucca/view/opengl_view.hpp>

#include <QOpenGLWidget>

namespace lucca_qt {

/**
 * Implements lucca::View interface.
 *
 * A lucca::QtOpenGLView always have a default camera,
 * created on constructor. You can add extra cameras.
 *
 * @see lucca::View
 *
 * @author O Programador
 */
class LUCCA_QT_API QtOpenGLView final : public lucca::OpenGLView, public QOpenGLWidget {
private:
	struct ThisIsPrivate final {};

public:
	QtOpenGLView(ThisIsPrivate);

	QtOpenGLView(std::unique_ptr<lucca::Painter> painter);
	QtOpenGLView(std::unique_ptr<lucca::Painter> painter, std::unique_ptr<lucca::Camera> camera);

	virtual ~QtOpenGLView();

private: // lucca::View interface
	virtual std::unique_ptr<View> clone() const final override;

	virtual void clearConnections(SizeType connectionsGroup) final override;

	virtual bool connectEventHandler(lucca::EventHandler& handler, SizeType connectionsGroup) final override;

	virtual bool setEventDispatcher(const std::shared_ptr<lucca::EventDispatcher>& dispatcher) final override;

private: // lucca::RenderTarget interface
	virtual bool hasCurrentRenderingContext() const final override;

	virtual void makeCurrentRenderingContext() final override;
	virtual void doneCurrentRenderingContext() final override;

	virtual SizeType getDefaultFramebufferObject() const final override;

	virtual SizeType getRenderTargetWidth() const final override;
	virtual SizeType getRenderTargetHeight() const final override;

	virtual void postRedisplay() final override;

private: // QOpenGLWidget interface
	virtual void initializeGL() final override;
	virtual void resizeGL(int width, int height) final override;
	virtual void paintGL() final override;

private:
	bool _setEventDispatcher(const std::shared_ptr<lucca::EventDispatcher>& dispatcher);

private:
	std::shared_ptr<lucca::EventDispatcher> _eventDispatcher;
};

}

#endif // LUCCA_QT_OPENGL_VIEW_HPP