#ifndef LUCCA_EVENT_HPP
#define LUCCA_EVENT_HPP

#include "lucca/config.hpp"

#include "lucca/event/keyboard_modifier.hpp"

namespace lucca {

/**
 * @author O Programador
 */
class LUCCA_API Event {
public:
	Event(KeyboardModifierFlags keyboardModifier);
	virtual ~Event() = 0;

	bool testKeyboardModifier(KeyboardModifier modifier) const;

private:
	const KeyboardModifierFlags _modifiers;
};

}

#endif // LUCCA_EVENT_HPP