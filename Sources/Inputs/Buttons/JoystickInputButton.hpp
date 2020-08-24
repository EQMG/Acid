#pragma once

#include "Devices/Joysticks.hpp"
#include "Inputs/InputButton.hpp"

namespace acid {
/**
 * @brief InputButton input from a joystick input device.
 */
class ACID_EXPORT JoystickInputButton : public InputButton::Registrar<JoystickInputButton> {
	static const bool Registered;
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param button The button on the joystick being checked.
	 */
	explicit JoystickInputButton(JoystickPort port = JoystickPort::_1, JoystickButton button = 0);

	bool IsDown() const override;

	InputAxis::ArgumentDescription GetArgumentDescription() const override;

	JoystickPort GetPort() const { return port; }
	void SetPort(JoystickPort port) { this->port = port; }

	JoystickButton GetButton() const { return button; }
	void SetButton(JoystickButton button) { this->button = button; }

	friend const Node &operator>>(const Node &node, JoystickInputButton &inputButton);
	friend Node &operator<<(Node &node, const JoystickInputButton &inputButton);

private:
	JoystickPort port;
	JoystickButton button;
};
}
