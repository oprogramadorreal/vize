#ifndef LUCCA_EVENT_SIGNALS_HPP
#define LUCCA_EVENT_SIGNALS_HPP

#include "lucca/config.hpp"

#include "lucca/event/key_event.hpp"
#include "lucca/event/mouse_event.hpp"

#include <boost/signals2.hpp>

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API EventSignalResult final {
public:
	inline static EventSignalResult defaultValue() { return EventSignalResult(); }
	inline static EventSignalResult stopPropagation() { return EventSignalResult(true); }

public:
	EventSignalResult();
	explicit EventSignalResult(bool stopPropagation);

public:
	bool getStopPropagation() const;

private:
	bool _stopPropagation = false;
};

/**
 * @see http://www.boost.org/doc/libs/1_48_0/doc/html/signals/tutorial.html#id3070223
 *
 * @author O Programador
 */
class EventSignalsCombiner final {
public:
	using result_type = EventSignalResult;

	template<class InputIterator>
	result_type operator()(InputIterator first, InputIterator last) const {
		for (auto it = first; it != last; ++it) {
			const auto slotResult = *it; // slot call

			if (slotResult.getStopPropagation()) {
				return slotResult;
			}
		}

		return result_type();
	}
};

using MousePressedSignal = boost::signals2::signal<EventSignalResult(const MouseEvent&), EventSignalsCombiner>;
using MousePressedSignalListener = MousePressedSignal::slot_function_type;

using MouseReleasedSignal = boost::signals2::signal<EventSignalResult(const MouseEvent&), EventSignalsCombiner>;
using MouseReleasedSignalListener = MouseReleasedSignal::slot_function_type;

using MouseDoubleClickSignal = boost::signals2::signal<EventSignalResult(const MouseEvent&), EventSignalsCombiner>;
using MouseDoubleClickSignalListener = MouseDoubleClickSignal::slot_function_type;

using MouseMovedSignal = boost::signals2::signal<EventSignalResult(const MouseEvent&), EventSignalsCombiner>;
using MouseMovedSignalListener = MouseMovedSignal::slot_function_type;

using MouseWheelSignal = boost::signals2::signal<EventSignalResult(const MouseWheelEvent&), EventSignalsCombiner>;
using MouseWheelSignalListener = MouseWheelSignal::slot_function_type;

using KeyPressedSignal = boost::signals2::signal<EventSignalResult(const KeyEvent&), EventSignalsCombiner>;
using KeyPressedSignalListener = KeyPressedSignal::slot_function_type;

using KeyReleasedSignal = boost::signals2::signal<EventSignalResult(const KeyEvent&), EventSignalsCombiner>;
using KeyReleasedSignalListener = KeyReleasedSignal::slot_function_type;

}

#endif // LUCCA_EVENT_SIGNALS_HPP