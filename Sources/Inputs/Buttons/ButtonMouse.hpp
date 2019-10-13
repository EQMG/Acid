#pragma once

#include "Devices/Mouse.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button input from the mouse input device.
 */
class ACID_EXPORT ButtonMouse : public Button::Registrar<ButtonMouse> {
public:
	/**
	 * Creates a new button mouse.
	 */
	explicit ButtonMouse() = default;

	/**
	 * Creates a new button mouse.
	 * @param button The button on the mouse being checked.
	 */
	explicit ButtonMouse(MouseButton button);

	bool IsDown() const override;

	MouseButton GetButton() const { return m_button; }
	void SetButton(MouseButton button) { m_button = button; }

private:
	MouseButton m_button;
};
}
