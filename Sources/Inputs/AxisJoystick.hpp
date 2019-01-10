#pragma once

#include "Devices/Joysticks.hpp"
#include "IAxis.hpp"

namespace acid
{
	/// <summary>
	/// Axis from a joystick.
	/// </summary>
	class ACID_EXPORT AxisJoystick :
		public IAxis
	{
	private:
		JoystickPort m_joystick;
		uint32_t m_axis;
		bool m_inverted;
	public:
		/// <summary>
		/// Creates a new axis joystick.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="axis"> The axis on the joystick being checked. </param>
		/// <param name="inverse"> If the axis direction should be inverted. </param>
		AxisJoystick(const JoystickPort &joystick, const uint32_t &axis, const bool &inverted = false);

		float GetAmount() const override;

		const JoystickPort &GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		const uint32_t &GetAxis() const { return m_axis; }

		void SetAxis(const uint32_t &axis) { m_axis = axis; }

		const bool &IsInverted() const { return m_inverted; }

		void SetInverted(const bool &inverted) { m_inverted = inverted; }
	};
}
