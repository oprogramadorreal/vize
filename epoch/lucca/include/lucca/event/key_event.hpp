#ifndef LUCCA_KEY_EVENT_HPP
#define LUCCA_KEY_EVENT_HPP

#include "lucca/config.hpp"

#include "lucca/event/event.hpp"

namespace lucca {

enum class Key {
	Return = 0,
	Escape,
	Delete,
	Z,
	Y,
	None
};

/**
 * @author O Programador
 */
class LUCCA_API KeyEvent final : public Event {
public:
	KeyEvent(Key key, KeyboardModifierFlags keyboardModifier);
	~KeyEvent();

public:
	Key getKey() const;

private:
	const Key _key;
};

}

#endif // LUCCA_KEY_EVENT_HPP