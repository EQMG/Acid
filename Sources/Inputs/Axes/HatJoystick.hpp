#pragma once

#include "Devices/Joysticks.hpp"
#include "Inputs/Buttons/Button.hpp"
#include "Axis.hpp"

namespace acid {
/**
 * @brief Button from a joystick.
 */
class ACID_EXPORT HatJoystick : public Axis::Registrar<HatJoystick>, public Button::Registrar<HatJoystick> {
	inline static const bool Registered = Axis::Registrar<HatJoystick>::Register("hatJoystick") &&
		Button::Registrar<HatJoystick>::Register("hatJoystick");
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param hat The hat that will be checked.
	 * @param hatFlags If this bit is found the hat will trigger {@link HatJoystick#IsDown}.
	 */
	explicit HatJoystick(JoystickPort port = 0, JoystickHat hat = 0, const BitMask<JoystickHatValue> &hatFlags = JoystickHatValue::Centered);

	ArgumentDescription GetArgumentDescription() const override;

	float GetAmount() const override;
	bool IsDown() const override;

	JoystickPort GetPort() const { return port; }
	void SetPort(JoystickPort port) { this->port = port; }

	JoystickHat GetHat() const { return hat; }
	void SetHat(JoystickHat hat) { this->hat = hat; }

	const BitMask<JoystickHatValue> &GetHatFlags() const { return hatFlags; }
	void SetHatFlags(JoystickHatValue hatFlags) { this->hatFlags = hatFlags; }

	friend const Node &operator>>(const Node &node, HatJoystick &hatJoystick);
	friend Node &operator<<(Node &node, const HatJoystick &hatJoystick);

private:
	JoystickPort port;
	JoystickHat hat;
	BitMask<JoystickHatValue> hatFlags;
	bool lastDown = false;
};
}
