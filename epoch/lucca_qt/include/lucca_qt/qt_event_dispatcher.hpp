#ifndef LUCCA_QT_EVENT_DISPATCHER_HPP
#define LUCCA_QT_EVENT_DISPATCHER_HPP

#include "lucca_qt/config.hpp"

#include <lucca/event/event_dispatcher.hpp>

#include <QObject>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

namespace lucca_qt {

/**
 * Must be installed as an event filter for a QObject.
 *
 * @author O Programador
 */
class LUCCA_QT_API QtEventDispatcher final : public lucca::EventDispatcher, public QObject {
private:
	virtual bool eventFilter(QObject* sourceObject, QEvent* event) final override;

private:
	bool _mousePressEvent(QObject* sourceObject, QMouseEvent* event) const;
	bool _mouseReleaseEvent(QObject* sourceObject, QMouseEvent* event) const;
	bool _mouseDoubleClick(QObject* sourceObject, QMouseEvent* event) const;
	bool _mouseMoveEvent(QObject* sourceObject, QMouseEvent* event) const;
	bool _mouseWheelEvent(QObject* sourceObject, QWheelEvent* event) const;
	bool _keyPressEvent(QObject* sourceObject, QKeyEvent* event) const;
	bool _keyReleasedEvent(QObject* sourceObject, QKeyEvent* event) const;
};

}

#endif // LUCCA_QT_EVENT_DISPATCHER_HPP