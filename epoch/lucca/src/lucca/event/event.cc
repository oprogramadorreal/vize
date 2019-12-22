#include "lucca/event/event.hpp"

namespace lucca {

Event::Event(KeyboardModifierFlags keyboardModifier)
	: _modifiers(keyboardModifier)
{ }

Event::~Event() = default;

bool Event::testKeyboardModifier(KeyboardModifier modifier) const {
	return _modifiers.testFlag(modifier);
}

}