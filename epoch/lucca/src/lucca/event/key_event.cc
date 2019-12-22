#include "lucca/event/key_event.hpp"

namespace lucca {

KeyEvent::KeyEvent(Key key, KeyboardModifierFlags keyboardModifier) 
	: Event(keyboardModifier), _key(key)
{ }

KeyEvent::~KeyEvent() = default;

Key KeyEvent::getKey() const {
	return _key;
}

}