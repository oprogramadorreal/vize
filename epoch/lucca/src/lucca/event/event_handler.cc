#include "lucca/event/event_handler.hpp"

#include "lucca/event/event_dispatcher.hpp"

namespace lucca {

EventHandler::EventHandler() = default;
EventHandler::~EventHandler() = default;

void EventHandler::connectToDispatcher(EventDispatcher& dispatcher, SizeType connectionsGroup) {
	if (const auto listener = _createMousePressedListener()) {
		_mousePressedConnection = dispatcher.connectToMousePressedSignal(listener, connectionsGroup);
	}
	if (const auto listener = _createMouseReleasedListener()) {
		_mouseReleasedConnection = dispatcher.connectToMouseReleasedSignal(listener, connectionsGroup);
	}
	if (const auto listener = _createMouseDoubleClickListener()) {
		_mouseDoubleClickConnection = dispatcher.connectToMouseDoubleClickSignal(listener, connectionsGroup);
	}
	if (const auto listener = _createMouseMovedListener()) {
		_mouseMovedConnection = dispatcher.connectToMouseMovedSignal(listener, connectionsGroup);
	}
	if (const auto listener = _createMouseWheelListener()) {
		_mouseWheelConnection = dispatcher.connectToMouseWheelSignal(listener, connectionsGroup);
	}
	if (const auto listener = _createKeyPressedListener()) {
		_keyPressedConnection = dispatcher.connectToKeyPressedSignal(listener, connectionsGroup);
	}
	if (const auto listener = _createKeyReleasedListener()) {
		_keyReleasedConnection = dispatcher.connectToKeyReleasedSignal(listener, connectionsGroup);
	}
}

void EventHandler::clearConnections() {
	_mousePressedConnection = boost::signals2::connection();
	_mouseReleasedConnection = boost::signals2::connection();
	_mouseDoubleClickConnection = boost::signals2::connection();
	_mouseMovedConnection = boost::signals2::connection();
	_mouseWheelConnection = boost::signals2::connection();
	_keyPressedConnection = boost::signals2::connection();
	_keyReleasedConnection = boost::signals2::connection();
}

MousePressedSignalListener EventHandler::_createMousePressedListener() {
	return MousePressedSignalListener();
}
MouseReleasedSignalListener EventHandler::_createMouseReleasedListener() {
	return MouseReleasedSignalListener();
}
MouseDoubleClickSignalListener EventHandler::_createMouseDoubleClickListener() {
	return MouseDoubleClickSignalListener();
}
MouseMovedSignalListener EventHandler::_createMouseMovedListener() {
	return MouseMovedSignalListener();
}
MouseWheelSignalListener EventHandler::_createMouseWheelListener() {
	return MouseWheelSignalListener();
}
KeyPressedSignalListener EventHandler::_createKeyPressedListener() {
	return KeyPressedSignalListener();
}
KeyReleasedSignalListener EventHandler::_createKeyReleasedListener() {
	return KeyReleasedSignalListener();
}

}