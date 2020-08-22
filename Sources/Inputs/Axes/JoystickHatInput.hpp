#pragma once

#include "Devices/Joysticks.hpp"
#include "Inputs/InputButton.hpp"
#include "Inputs/InputAxis.hpp"

namespace acid {
/**
 * @brief InputButton from a joystick.
 */
class ACID_EXPORT JoystickHatInput : public InputAxis::Registrar<JoystickHatInput>, public InputButton::Registrar<JoystickHatInput> {
	inline static const bool Registered = InputAxis::Registrar<JoystickHatInput>::Register("joystickHat") &&
		InputButton::Registrar<JoystickHatInput>::Register("joystickHat");
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param hat The hat that will be checked.
	 * @param hatFlags If this bit is found the hat will trigger {@link JoystickHatInput#IsDown}.
	 */
	explicit JoystickHatInput(JoystickPort port = JoystickPort::_1, JoystickHat hat = 0, const bitmask::bitmask<JoystickHatValue> &hatFlags = JoystickHatValue::Centered);

	ArgumentDescription GetArgumentDescription() const override;

	float GetAmount() const override;
	bool IsDown() const override;

	JoystickPort GetPort() const { return port; }
	void SetPort(JoystickPort port) { this->port = port; }

	JoystickHat GetHat() const { return hat; }
	void SetHat(JoystickHat hat) { this->hat = hat; }

	const bitmask::bitmask<JoystickHatValue> &GetHatFlags() const { return hatFlags; }
	void SetHatFlags(JoystickHatValue hatFlags) { this->hatFlags = hatFlags; }

	friend const Node &operator>>(const Node &node, JoystickHatInput &input);
	friend Node &operator<<(Node &node, const JoystickHatInput &input);

private:
	JoystickPort port;
	JoystickHat hat;
	bitmask::bitmask<JoystickHatValue> hatFlags;
	bool lastDown = false;
};

using JoystickHatInputAxis = JoystickHatInput;
using JoystickHatInputButton = JoystickHatInput;
}
