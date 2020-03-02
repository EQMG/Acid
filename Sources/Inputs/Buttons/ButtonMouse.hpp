#pragma once

#include "Devices/Mouse.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button input from the mouse input device.
 */
class ACID_EXPORT ButtonMouse : public Button::Registrar<ButtonMouse> {
	inline static const bool Registered = Register("buttonMouse");
public:
	/**
	 * Creates a new button mouse.
	 * @param button The button on the mouse being checked.
	 */
	explicit ButtonMouse(MouseButton button = MouseButton::First);

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	MouseButton GetButton() const { return button; }
	void SetButton(MouseButton button) { this->button = button; }

	friend const Node &operator>>(const Node &node, ButtonMouse &buttonMouse);
	friend Node &operator<<(Node &node, const ButtonMouse &buttonMouse);

private:
	MouseButton button;
};
}
