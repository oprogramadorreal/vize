#include "lucca/event/event_signals.hpp"

namespace lucca {

EventSignalResult::EventSignalResult()
	: EventSignalResult(false)
{ }

EventSignalResult::EventSignalResult(bool stopPropagation)
	: _stopPropagation(stopPropagation)
{ }

bool EventSignalResult::getStopPropagation() const {
	return _stopPropagation;
}

}