#pragma once

#include "IAxis.hpp"

namespace acid
{
/// <summary>
/// Axis from a joystick.
/// </summary>
class ACID_EXPORT AxisJoystick :
	public IAxis
{
public:
	/// <summary>
	/// Creates a new axis joystick.
	/// </summary>
	/// <param name="port"> The joystick port. </param>
	/// <param name="axis"> The axis on the joystick being checked. </param>
	/// <param name="inverse"> If the axis direction should be inverted. </param>
	AxisJoystick(const uint32_t &port, const uint32_t &axis, const bool &inverted = false);

	float GetAmount() const override;

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
