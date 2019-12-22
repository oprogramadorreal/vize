#include "lucca/event/mouse_event.hpp"

namespace lucca {

MouseEvent::MouseEvent(MouseButton button, Float x, Float y, const View* sourceView, KeyboardModifierFlags keyboardModifier)
	: ViewEvent(x, y, sourceView, keyboardModifier), _button(button)
{ }

MouseEvent::~MouseEvent() = default;

MouseButton MouseEvent::getButton() const {
	return _button;
}

///////////////////////////////////////////

MouseWheelEvent::MouseWheelEvent(Float movement, Float x, Float y, const View* sourceView, KeyboardModifierFlags keyboardModifier)
	: ViewEvent(x, y, sourceView, keyboardModifier), _movement(movement)
{ }

MouseWheelEvent::~MouseWheelEvent() = default;

Float MouseWheelEvent::getMovement() const {
	return _movement;
}

}