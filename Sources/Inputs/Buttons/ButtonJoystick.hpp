#pragma once

#include "Button.hpp"
#include "Devices/Joysticks.hpp"

namespace acid {
/**
 * @brief Button input from a joystick input device.
 */
class ACID_EXPORT ButtonJoystick : public Button::Registrar<ButtonJoystick> {
public:
	/**
	 * Creates a new joystick button.
	 */
	ButtonJoystick() = default;

	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param button The button on the joystick being checked.
	 */
	ButtonJoystick(JoystickPort port, JoystickButton button);

	bool IsDown() const override;

	JoystickPort GetPort() const { return m_port; }
	void SetPort(JoystickPort port) { m_port = port; }

	JoystickButton GetButton() const { return m_button; }
	void SetButton(JoystickButton button) { m_button = button; }

private:
	JoystickPort m_port;
	JoystickButton m_button;
};
}
