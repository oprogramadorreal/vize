#include "lucca_qt/qt_opengl_view.hpp"

#include "lucca_qt/qt_event_dispatcher.hpp"

#include <lucca/camera.hpp>
#include <lucca/painter/painter.hpp>
#include <lucca/event/event_handler.hpp>

#include <QOpenGLContext>

namespace lucca_qt {

QtOpenGLView::QtOpenGLView(ThisIsPrivate) {
	QSurfaceFormat format;
	format.setSamples(4);
	//format.setSwapInterval(0);
	setFormat(format);

	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);

	_setEventDispatcher(std::make_shared<QtEventDispatcher>());
}

QtOpenGLView::QtOpenGLView(std::unique_ptr<lucca::Painter> painter, std::unique_ptr<lucca::Camera> camera)
	: QtOpenGLView(ThisIsPrivate())
{
	_setPainter(std::move(painter));
	_addCamera(std::move(camera));
}

QtOpenGLView::QtOpenGLView(std::unique_ptr<lucca::Painter> painter)
	: QtOpenGLView(std::move(painter), std::make_unique<lucca::Camera>())
{ }

QtOpenGLView::~QtOpenGLView() {
	_dispose();
}

std::unique_ptr<lucca::View> QtOpenGLView::clone() const {
	auto copy = std::make_unique<QtOpenGLView>(ThisIsPrivate());
	copy->_copyFrom(*this);
	return std::move(copy);
}

void QtOpenGLView::clearConnections(SizeType connectionsGroup) {
	if (_eventDispatcher) {
		_eventDispatcher->clearConnections(connectionsGroup);
	}
}

bool QtOpenGLView::connectEventHandler(lucca::EventHandler& handler, SizeType connectionsGroup) {
	if (!_eventDispatcher) {
		return false;
	}

	handler.connectToDispatcher(*_eventDispatcher, connectionsGroup);

	return true;
}

bool QtOpenGLView::_setEventDispatcher(const std::shared_ptr<lucca::EventDispatcher>& dispatcher) {
	const auto qtDispatcher = std::dynamic_pointer_cast<QtEventDispatcher>(dispatcher);

	if (!qtDispatcher) {
		return false; // this dispatcher cannot be installed
	}

	if (const auto currentQtDispatcher = dynamic_cast<QObject*>(_eventDispatcher.get())) {
		removeEventFilter(currentQtDispatcher);
	}

	_eventDispatcher = dispatcher;
	installEventFilter(qtDispatcher.get());

	return true;
}

bool QtOpenGLView::setEventDispatcher(const std::shared_ptr<lucca::EventDispatcher>& dispatcher) {
	return _setEventDispatcher(dispatcher);
}

bool QtOpenGLView::hasCurrentRenderingContext() const {
	return context() == QOpenGLContext::currentContext();
}

void QtOpenGLView::makeCurrentRenderingContext() {
	makeCurrent();
	assert(hasCurrentRenderingContext());
}

void QtOpenGLView::doneCurrentRenderingContext() {
	doneCurrent();
	assert(!hasCurrentRenderingContext());
}

SizeType QtOpenGLView::getDefaultFramebufferObject() const {
	return QOpenGLWidget::defaultFramebufferObject();
}

SizeType QtOpenGLView::getRenderTargetWidth() const {
	return SizeType(width());
}

SizeType QtOpenGLView::getRenderTargetHeight() const {
	return SizeType(height());
}

void QtOpenGLView::postRedisplay() {
	QOpenGLWidget::update();
}

void QtOpenGLView::initializeGL() {
	_initialize();
}

void QtOpenGLView::resizeGL(int width, int height) {
	_resize(SizeType(width), SizeType(height));
}

void QtOpenGLView::paintGL() {
	_paint();
}

}