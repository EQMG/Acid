#pragma once

#include "IButton.hpp"

namespace acid
{
/**
 * @brief Button input from a joystick input device.
 */
class ACID_EXPORT ButtonJoystick :
	public IButton
{
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param button The button on the joystick being checked.
	 */
	ButtonJoystick(const uint32_t &port, const uint32_t &button);

	bool IsDown() const override;

	const uint32_t &GetPort() const { return m_port; }

	void SetPort(const uint32_t &port) { m_port = port; }

	const uint32_t &GetButton() const { return m_button; }

	void SetButton(const uint32_t &button) { m_button = button; }

private:
	uint32_t m_port;
	uint32_t m_button;
};
}
