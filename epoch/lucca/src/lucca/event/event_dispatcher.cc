#include "lucca/event/event_dispatcher.hpp"

namespace lucca {

EventDispatcher::EventDispatcher() = default;
EventDispatcher::~EventDispatcher() = default;

void EventDispatcher::clearConnections(SizeType connectionGroup) {
	_mousePressed.disconnect(MousePressedSignal::group_type(connectionGroup));
	_mouseReleased.disconnect(MouseReleasedSignal::group_type(connectionGroup));
	_mouseDoubleClick.disconnect(MouseDoubleClickSignal::group_type(connectionGroup));
	_mouseMoved.disconnect(MouseMovedSignal::group_type(connectionGroup));
	_mouseWheel.disconnect(MouseWheelSignal::group_type(connectionGroup));
	_keyPressed.disconnect(KeyPressedSignal::group_type(connectionGroup));
	_keyReleased.disconnect(KeyReleasedSignal::group_type(connectionGroup));
}

boost::signals2::connection EventDispatcher::connectToMousePressedSignal(MousePressedSignalListener listener, SizeType connectionGroup) {
	return _mousePressed.connect(MousePressedSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}
boost::signals2::connection EventDispatcher::connectToMouseReleasedSignal(MouseReleasedSignalListener listener, SizeType connectionGroup) {
	return _mouseReleased.connect(MouseReleasedSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}
boost::signals2::connection EventDispatcher::connectToMouseDoubleClickSignal(MouseMovedSignalListener listener, SizeType connectionGroup) {
	return _mouseDoubleClick.connect(MouseDoubleClickSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}
boost::signals2::connection EventDispatcher::connectToMouseMovedSignal(MouseMovedSignalListener listener, SizeType connectionGroup) {
	return _mouseMoved.connect(MouseMovedSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}
boost::signals2::connection EventDispatcher::connectToMouseWheelSignal(MouseWheelSignalListener listener, SizeType connectionGroup) {
	return _mouseWheel.connect(MouseWheelSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}
boost::signals2::connection EventDispatcher::connectToKeyPressedSignal(KeyPressedSignalListener listener, SizeType connectionGroup) {
	return _keyPressed.connect(KeyPressedSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}
boost::signals2::connection EventDispatcher::connectToKeyReleasedSignal(KeyReleasedSignalListener listener, SizeType connectionGroup) {
	return _keyReleased.connect(KeyReleasedSignal::group_type(connectionGroup), listener, boost::signals2::at_back);
}

EventSignalResult EventDispatcher::emitMousePressed(const MouseEvent& event) const {
	return _mousePressed(event);
}
EventSignalResult EventDispatcher::emitMouseReleased(const MouseEvent& event) const {
	return _mouseReleased(event);
}
EventSignalResult EventDispatcher::emitMouseDoubleClick(const MouseEvent& event) const {
	return _mouseDoubleClick(event);
}
EventSignalResult EventDispatcher::emitMouseMoved(const MouseEvent& event) const {
	return _mouseMoved(event);
}
EventSignalResult EventDispatcher::emitMouseWheel(const MouseWheelEvent& event) const {
	return _mouseWheel(event);
}
EventSignalResult EventDispatcher::emitKeyPressed(const KeyEvent& event) const {
	return _keyPressed(event);
}
EventSignalResult EventDispatcher::emitKeyReleased(const KeyEvent& event) const {
	return _keyReleased(event);
}

}