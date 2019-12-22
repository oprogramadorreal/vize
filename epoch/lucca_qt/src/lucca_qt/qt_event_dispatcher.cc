#include "lucca_qt/qt_event_dispatcher.hpp"

#include <lucca/event/mouse_event.hpp>
#include <lucca/event/key_event.hpp>
#include <lucca/view/view.hpp>

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <boost/polymorphic_cast.hpp>

namespace lucca_qt {

bool QtEventDispatcher::eventFilter(QObject* sourceObject, QEvent* event) {
	assert(event);

	switch (event->type()) {
		case QEvent::MouseButtonPress:
			return _mousePressEvent(sourceObject, boost::polymorphic_downcast<QMouseEvent*>(event));
		case QEvent::MouseButtonRelease:
			return _mouseReleaseEvent(sourceObject, boost::polymorphic_downcast<QMouseEvent*>(event));
		case QEvent::MouseButtonDblClick:
			return _mouseDoubleClick(sourceObject, boost::polymorphic_downcast<QMouseEvent*>(event));
		case QEvent::MouseMove:
			return _mouseMoveEvent(sourceObject, boost::polymorphic_downcast<QMouseEvent*>(event));
		case QEvent::Wheel:
			return _mouseWheelEvent(sourceObject, boost::polymorphic_downcast<QWheelEvent*>(event));
		case QEvent::KeyPress:
			return _keyPressEvent(sourceObject, boost::polymorphic_downcast<QKeyEvent*>(event));
		case QEvent::KeyRelease:
			return _keyReleasedEvent(sourceObject, boost::polymorphic_downcast<QKeyEvent*>(event));
		default:;
	}

	return QObject::eventFilter(sourceObject, event);
}

namespace {

lucca::MouseButton _getMouseButton(Qt::MouseButton qtMouseButton) {
	switch (qtMouseButton) {
		case Qt::NoButton:
			return lucca::MouseButton::None;
		case Qt::LeftButton:
			return lucca::MouseButton::Left;
		case Qt::RightButton:
			return lucca::MouseButton::Right;
		case Qt::MiddleButton:
			return lucca::MouseButton::Middle;
		default:;
	}

	return lucca::MouseButton::None;
}

lucca::KeyboardModifierFlags _getKeyboardModifiers(Qt::KeyboardModifiers qtModifiers) {
	lucca::KeyboardModifierFlags modifiers;

	if (qtModifiers.testFlag(Qt::ControlModifier)) {
		modifiers.addFlag(lucca::KeyboardModifier::Control);
	}
	if (qtModifiers.testFlag(Qt::ShiftModifier)) {
		modifiers.addFlag(lucca::KeyboardModifier::Shift);
	}
	if (qtModifiers.testFlag(Qt::AltModifier)) {
		modifiers.addFlag(lucca::KeyboardModifier::Alt);
	}

	return modifiers;
}

}

bool QtEventDispatcher::_mousePressEvent(QObject* sourceObject, QMouseEvent* event) const {
	assert(event);
	const auto view = dynamic_cast<lucca::View*>(sourceObject);
	emitMousePressed(lucca::MouseEvent(_getMouseButton(event->button()), Float(event->x()), Float(event->y()), view, _getKeyboardModifiers(event->modifiers())));
	return true;
}

bool QtEventDispatcher::_mouseReleaseEvent(QObject* sourceObject, QMouseEvent* event) const {
	assert(event);
	const auto view = dynamic_cast<lucca::View*>(sourceObject);
	assert(view);
	emitMouseReleased(lucca::MouseEvent(_getMouseButton(event->button()), Float(event->x()), Float(event->y()), view, _getKeyboardModifiers(event->modifiers())));
	return true;
}

bool QtEventDispatcher::_mouseDoubleClick(QObject* sourceObject, QMouseEvent* event) const {
	assert(event);
	const auto view = dynamic_cast<lucca::View*>(sourceObject);
	assert(view);
	emitMouseDoubleClick(lucca::MouseEvent(_getMouseButton(event->button()), Float(event->x()), Float(event->y()), view, _getKeyboardModifiers(event->modifiers())));
	return true;
}

bool QtEventDispatcher::_mouseMoveEvent(QObject* sourceObject, QMouseEvent* event) const {
	assert(event);

	auto dragButton = Qt::NoButton;

	if (event->buttons() & Qt::LeftButton) {
		dragButton = Qt::LeftButton;
	} else if (event->buttons() & Qt::RightButton) {
		dragButton = Qt::RightButton;
	} else if (event->buttons() & Qt::MiddleButton) {
		dragButton = Qt::MiddleButton;
	}

	const auto view = dynamic_cast<lucca::View*>(sourceObject);
	assert(view);

	emitMouseMoved(lucca::MouseEvent(_getMouseButton(dragButton), Float(event->x()), Float(event->y()), view, _getKeyboardModifiers(event->modifiers())));

	return true;
}

bool QtEventDispatcher::_mouseWheelEvent(QObject* sourceObject, QWheelEvent* event) const {
	assert(event);
	const auto view = dynamic_cast<lucca::View*>(sourceObject);
	assert(view);
	emitMouseWheel(lucca::MouseWheelEvent(Float(event->angleDelta().y()), Float(event->x()), Float(event->y()), view, _getKeyboardModifiers(event->modifiers())));
	return true;
}

namespace {

lucca::Key _qtKeyToKey(Qt::Key qtKey) {
	switch (qtKey) {
		case Qt::Key_Return: return lucca::Key::Return;
		case Qt::Key_Escape: return lucca::Key::Escape;
		case Qt::Key_Delete: return lucca::Key::Delete;
		case Qt::Key_Z: return lucca::Key::Z;
		case Qt::Key_Y: return lucca::Key::Y;
		default:;
	};

	return lucca::Key::None;
};

}

bool QtEventDispatcher::_keyPressEvent(QObject*, QKeyEvent* event) const {
	assert(event);
	const auto qtKey = Qt::Key(event->key());
	const auto key = _qtKeyToKey(qtKey);
	emitKeyPressed(lucca::KeyEvent(key, _getKeyboardModifiers(event->modifiers())));
	return true;
}

bool QtEventDispatcher::_keyReleasedEvent(QObject* sourceObject, QKeyEvent* event) const {
	assert(event);
	const auto qtKey = Qt::Key(event->key());
	const auto key = _qtKeyToKey(qtKey);
	emitKeyReleased(lucca::KeyEvent(key, _getKeyboardModifiers(event->modifiers())));
	return true;
}

}