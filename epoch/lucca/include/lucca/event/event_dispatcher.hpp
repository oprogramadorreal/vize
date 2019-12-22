#ifndef LUCCA_EVENT_DISPATCHER_HPP
#define LUCCA_EVENT_DISPATCHER_HPP

#include "lucca/config.hpp"

#include "lucca/event/event_signals.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API EventDispatcher : boost::noncopyable {
public:
	EventDispatcher();
	virtual ~EventDispatcher() = 0;

public:
	void clearConnections(SizeType connectionGroup);

	boost::signals2::connection connectToMousePressedSignal(MousePressedSignalListener listener, SizeType connectionGroup);
	boost::signals2::connection connectToMouseReleasedSignal(MouseReleasedSignalListener listener, SizeType connectionGroup);
	boost::signals2::connection connectToMouseDoubleClickSignal(MouseMovedSignalListener listener, SizeType connectionGroup);
	boost::signals2::connection connectToMouseMovedSignal(MouseMovedSignalListener listener, SizeType connectionGroup);
	boost::signals2::connection connectToMouseWheelSignal(MouseWheelSignalListener listener, SizeType connectionGroup);
	boost::signals2::connection connectToKeyPressedSignal(KeyPressedSignalListener listener, SizeType connectionGroup);
	boost::signals2::connection connectToKeyReleasedSignal(KeyReleasedSignalListener listener, SizeType connectionGroup);

	EventSignalResult emitMousePressed(const MouseEvent& event) const;
	EventSignalResult emitMouseReleased(const MouseEvent& event) const;
	EventSignalResult emitMouseDoubleClick(const MouseEvent& event) const;
	EventSignalResult emitMouseMoved(const MouseEvent& event) const;
	EventSignalResult emitMouseWheel(const MouseWheelEvent& event) const;
	EventSignalResult emitKeyPressed(const KeyEvent& event) const;
	EventSignalResult emitKeyReleased(const KeyEvent& event) const;

private:
	MousePressedSignal _mousePressed;
	MouseReleasedSignal _mouseReleased;
	MouseDoubleClickSignal _mouseDoubleClick;
	MouseMovedSignal _mouseMoved;
	MouseWheelSignal _mouseWheel;
	KeyPressedSignal _keyPressed;
	KeyReleasedSignal _keyReleased;
};

}

#endif // LUCCA_EVENT_DISPATCHER_HPP