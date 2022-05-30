#pragma once

#include "Devices/Joysticks.hpp"
#include "Inputs/InputButton.hpp"

namespace acid {
/**
 * @brief InputButton input from a joystick input device.
 */
class ACID_EXPORT JoystickInputButton : public InputButton::Registrar<JoystickInputButton> {
	inline static const bool Registered = Register("joystick");
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param button The button on the joystick being checked.
	 */
	explicit JoystickInputButton(JoystickPort port = JoystickPort::_1, JoystickButton button = 0);

	bool IsDown() const override;

	InputAxis::ArgumentDescription GetArgumentDescription() const override;

	bool IsConnected() const { return joystick->IsConnected(); }

	JoystickPort GetPort() const { return joystick->GetPort(); }
	void SetPort(JoystickPort port);

	JoystickButton GetButton() const { return button; }
	void SetButton(JoystickButton button) { this->button = button; }

	friend const Node &operator>>(const Node &node, JoystickInputButton &inputButton);
	friend Node &operator<<(Node &node, const JoystickInputButton &inputButton);

private:
	Joystick *joystick;
	JoystickButton button;
};
}
