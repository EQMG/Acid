#pragma once

#include "Axis.hpp"

namespace acid
{
/**
 * @brief Axis input from a joystick input device.
 */
class ACID_EXPORT AxisJoystick :
	public Axis
{
public:
	/**
	 * Creates a new axis joystick.
	 * @param port The joystick port.
	 * @param axis The axis on the joystick being checked.
	 * @param inverted If the axis direction should be inverted.
	 */
	AxisJoystick(const uint32_t &port, const uint32_t &axis, const bool &inverted = false);

	float GetAmount() const override;

	bool IsConnected() const;

	const uint32_t &GetPort() const { return m_port; }

	void SetPort(const uint32_t &port) { m_port = port; }

	const uint32_t &GetAxis() const { return m_axis; }

	void SetAxis(const uint32_t &axis) { m_axis = axis; }

	const bool &IsInverted() const { return m_inverted; }

	void SetInverted(const bool &inverted) { m_inverted = inverted; }

private:
	uint32_t m_port;
	uint32_t m_axis;
	bool m_inverted;
};
}
