#ifndef LUCCA_KEYBOARD_MODIFIER_HPP
#define LUCCA_KEYBOARD_MODIFIER_HPP

#include "lucca/config.hpp"

#include "lucca/enum_flags.hpp"

namespace lucca {

/**
 * @author O Programador
 */
enum class KeyboardModifier {
	None = 0x01,
	Shift = 0x02,
	Control = 0x04,
	Alt = 0x08
};

/**
 * @author O Programador
 */
using KeyboardModifierFlags = EnumFlags<KeyboardModifier>;

}

#endif // LUCCA_KEYBOARD_MODIFIER_HPP