#pragma once

#include "Devices/Joysticks.hpp"
#include "Axis.hpp"
#include "Button.hpp"

namespace acid
{
/**
 * @brief Button from a joystick.
 */
class HatJoystick :
	public Axis,
	public Button
{
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param hat The hat that will be checked.
	 * @param hatFlags If this bit is found the hat will trigger {@link HatJoystick#IsDown}.
	 */
	HatJoystick(const uint32_t &port, const uint32_t &hat, const BitMask<JoystickHat> &hatFlags = JoystickHat::Centered);

	float GetAmount() const override;

	bool IsDown() const override;

	const uint32_t &GetPort() const { return m_port; }

	void SetPort(const uint32_t &port) { m_port = port; }

	const uint32_t &GetHat() const { return m_hat; }

	void SetHat(const uint32_t &hat) { m_hat = hat; }

	const BitMask<JoystickHat> &GetHatFlags() const { return m_hatFlags; }

	void SetHatFlags(const JoystickHat &hatFlags) { m_hatFlags = hatFlags; }

private:
	uint32_t m_port;
	uint32_t m_hat;
	BitMask<JoystickHat> m_hatFlags;
	bool m_lastDown;
};
}
