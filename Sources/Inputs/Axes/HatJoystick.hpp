#pragma once

#include "Devices/Joysticks.hpp"
#include "Inputs/Buttons/Button.hpp"
#include "Axis.hpp"

namespace acid {
/**
 * @brief Button from a joystick.
 */
class ACID_EXPORT HatJoystick : public Axis::Registrar<HatJoystick>, public Button {
public:
	/**
	 * Creates a new joystick button.
	 */
	HatJoystick() = default;

	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param hat The hat that will be checked.
	 * @param hatFlags If this bit is found the hat will trigger {@link HatJoystick#IsDown}.
	 */
	HatJoystick(JoystickPort port, JoystickHat hat, const BitMask<JoystickHatValue> &hatFlags = JoystickHatValue::Centered);

	float GetAmount() const override;
	bool IsDown() const override;

	JoystickPort GetPort() const { return m_port; }
	void SetPort(JoystickPort port) { m_port = port; }

	JoystickHat GetHat() const { return m_hat; }
	void SetHat(JoystickHat hat) { m_hat = hat; }

	const BitMask<JoystickHatValue> &GetHatFlags() const { return m_hatFlags; }
	void SetHatFlags(JoystickHatValue hatFlags) { m_hatFlags = hatFlags; }

private:
	JoystickPort m_port;
	JoystickHat m_hat;
	BitMask<JoystickHatValue> m_hatFlags;
	bool m_lastDown = false;
};
}
