#pragma once

#include "Devices/Mouse.hpp"
#include "Button.hpp"

namespace acid {
/**
 * @brief Button input from the mouse input device.
 */
class ACID_EXPORT
ButtonMouse
:
public
Button
{
public:
	/**
	 * Creates a new button mouse.
	 * @param button The button on the mouse being checked.
	 */
	explicit ButtonMouse(const MouseButton &button);

	bool IsDown() const override;

	const MouseButton &GetButton() const { return m_button; }

	void SetButton(const MouseButton &button) { m_button = button; }

private:
	MouseButton m_button;
};
}
