#ifndef LUCCA_MOUSE_EVENT_HPP
#define LUCCA_MOUSE_EVENT_HPP

#include "lucca/config.hpp"

#include "lucca/event/view_event.hpp"

namespace lucca {

/**
 * @author O Programador
 */
enum class MouseButton {
	None = 0x01,
	Left = 0x02,
	Right = 0x04,
	Middle = 0x08
};

using MouseButtonFlags = EnumFlags<MouseButton>;

/**
 * @author O Programador
 */
class LUCCA_API MouseEvent final : public ViewEvent {
public:
	MouseEvent(MouseButton button, Float x, Float y, const View* sourceView, KeyboardModifierFlags keyboardModifier);
	virtual ~MouseEvent();

	MouseButton getButton() const;
	
private:
	const MouseButton _button;
};

/**
 * @author O Programador
 */
class LUCCA_API MouseWheelEvent final : public ViewEvent {
public:
	MouseWheelEvent(Float movement, Float x, Float y, const View* sourceView, KeyboardModifierFlags keyboardModifier);
	virtual ~MouseWheelEvent();

	Float getMovement() const;

private:
	const Float _movement;
};

}

#endif // LUCCA_MOUSE_EVENT_HPP