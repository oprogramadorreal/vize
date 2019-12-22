#ifndef LUCCA_EVENT_HANDLER_HPP
#define LUCCA_EVENT_HANDLER_HPP

#include "lucca/config.hpp"

#include "lucca/event/event_signals.hpp"

#include <boost/noncopyable.hpp>

namespace lucca {

class EventDispatcher;

/**
 * @see lucca::TrackballCameraController
 *
 * @author O Programador
 */
class LUCCA_API EventHandler : boost::noncopyable {
public:
	EventHandler();
	virtual ~EventHandler() = 0;

public:
	void connectToDispatcher(EventDispatcher& dispatcher, SizeType connectionsGroup);

	/**
	 * Stop handling all registered signals.
	 */
	void clearConnections();

private:
	virtual MousePressedSignalListener _createMousePressedListener();
	virtual MouseReleasedSignalListener _createMouseReleasedListener();
	virtual MouseDoubleClickSignalListener _createMouseDoubleClickListener();
	virtual MouseMovedSignalListener _createMouseMovedListener();
	virtual MouseWheelSignalListener _createMouseWheelListener();
	virtual KeyPressedSignalListener _createKeyPressedListener();
	virtual KeyReleasedSignalListener _createKeyReleasedListener();

private:
	boost::signals2::scoped_connection _mousePressedConnection;
	boost::signals2::scoped_connection _mouseReleasedConnection;
	boost::signals2::scoped_connection _mouseDoubleClickConnection;
	boost::signals2::scoped_connection _mouseMovedConnection;
	boost::signals2::scoped_connection _mouseWheelConnection;
	boost::signals2::scoped_connection _keyPressedConnection;
	boost::signals2::scoped_connection _keyReleasedConnection;
};

}

#endif // LUCCA_EVENT_HANDLER_HPP