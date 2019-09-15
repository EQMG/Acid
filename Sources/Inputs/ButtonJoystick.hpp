#pragma once

#include "Button.hpp"

namespace acid {
/**
 * @brief Button input from a joystick input device.
 */
class ACID_EXPORT ButtonJoystick : public Button {
public:
	/**
	 * Creates a new joystick button.
	 * @param port The joystick port.
	 * @param button The button on the joystick being checked.
	 */
	ButtonJoystick(uint32_t port, uint32_t button);

	bool IsDown() const override;

	uint32_t GetPort() const { return m_port; }
	void SetPort(uint32_t port) { m_port = port; }

	uint32_t GetButton() const { return m_button; }
	void SetButton(uint32_t button) { m_button = button; }

private:
	uint32_t m_port;
	uint32_t m_button;
};
}
