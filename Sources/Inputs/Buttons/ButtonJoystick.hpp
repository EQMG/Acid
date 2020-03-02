#pragma once

#include "Button.hpp"
#include "Devices/Joysticks.hpp"

namespace acid {
/**
 * @brief Button input from a joystick input device.
 */
class ACID_EXPORT ButtonJoystick : public Button::Registrar<ButtonJoystick> {
	inline static const bool Registered = Register("buttonJoystick");
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param button The button on the joystick being checked.
	 */
	explicit ButtonJoystick(JoystickPort port = 0, JoystickButton button = 0);

	bool IsDown() const override;

	Axis::ArgumentDescription GetArgumentDescription() const override;

	JoystickPort GetPort() const { return port; }
	void SetPort(JoystickPort port) { this->port = port; }

	JoystickButton GetButton() const { return button; }
	void SetButton(JoystickButton button) { this->button = button; }

	friend const Node &operator>>(const Node &node, ButtonJoystick &buttonJoystick);
	friend Node &operator<<(Node &node, const ButtonJoystick &buttonJoystick);

private:
	JoystickPort port;
	JoystickButton button;
};
}
