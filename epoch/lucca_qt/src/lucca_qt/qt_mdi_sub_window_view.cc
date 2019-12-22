#include "lucca_qt/qt_mdi_sub_window_view.hpp"

#include "lucca_qt/qt_opengl_view.hpp"

#include <lucca/camera.hpp>
#include <lucca/painter/painter.hpp>
#include <lucca/view/view_composite.hpp>

#include <QIcon>
#include <QMdiArea>

namespace lucca_qt {

QtMdiSubWindowView::QtMdiSubWindowView(QtOpenGLView& widget)
	: _widgetView(widget)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowIcon(QIcon(" "));
}

QtMdiSubWindowView::~QtMdiSubWindowView() = default;

lucca::View& QtMdiSubWindowView::getWidgetView() const {
	return _widgetView;
}

void QtMdiSubWindowView::setWidgetViewCursor(const QCursor& cursor) {
	_widgetView.setCursor(cursor);
}

void QtMdiSubWindowView::setPainter(std::unique_ptr<lucca::Painter> painter) {
	getWidgetView().setPainter(std::move(painter));
}

lucca::Painter* QtMdiSubWindowView::getPainter() const {
	return getWidgetView().getPainter();
}

lucca::Camera* QtMdiSubWindowView::getDefaultCamera() const {
	return getWidgetView().getDefaultCamera();
}

void QtMdiSubWindowView::addCamera(std::unique_ptr<lucca::Camera> camera) {
	getWidgetView().addCamera(std::move(camera));
}

void QtMdiSubWindowView::clearConnections(SizeType connectionsGroup) {
	getWidgetView().clearConnections(connectionsGroup);
}

bool QtMdiSubWindowView::connectEventHandler(lucca::EventHandler& handler, SizeType connectionsGroup) {
	return getWidgetView().connectEventHandler(handler, connectionsGroup);
}

bool QtMdiSubWindowView::setEventDispatcher(const std::shared_ptr<lucca::EventDispatcher>& dispatcher) {
	return getWidgetView().setEventDispatcher(dispatcher);
}

lucca::ViewComposite* QtMdiSubWindowView::getComposite() {
	return dynamic_cast<lucca::ViewComposite*>(mdiArea());
}

bool QtMdiSubWindowView::hasCurrentRenderingContext() const {
	return getWidgetView().hasCurrentRenderingContext();
}

void QtMdiSubWindowView::makeCurrentRenderingContext() {
	getWidgetView().makeCurrentRenderingContext();
}

void QtMdiSubWindowView::doneCurrentRenderingContext() {
	getWidgetView().doneCurrentRenderingContext();
}

SizeType QtMdiSubWindowView::getDefaultFramebufferObject() const {
	return getWidgetView().getDefaultFramebufferObject();
}

SizeType QtMdiSubWindowView::getRenderTargetWidth() const {
	return getWidgetView().getRenderTargetWidth();
}

SizeType QtMdiSubWindowView::getRenderTargetHeight() const {
	return getWidgetView().getRenderTargetHeight();
}

void QtMdiSubWindowView::postRedisplay() {
	return getWidgetView().postRedisplay();
}

}