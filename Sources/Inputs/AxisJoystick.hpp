#pragma once

#include <vector>
#include "Joysticks.hpp"
#include "IAxis.hpp"

namespace fl
{
	/// <summary>
	/// Axis from a joystick.
	/// </summary>
	class FL_EXPORT AxisJoystick :
		public IAxis
	{
	private:
		JoystickPort m_joystick;
		std::vector<int> m_axes;
		bool m_reverse;
	public:
		/// <summary>
		/// Creates a new axis joystick.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="axes"> The axes on the joystick being checked. </param>
		/// <param name="reverse"> If the axis direction should be inverted. </param>
		AxisJoystick(const JoystickPort &joystick, const std::vector<int> &axes, const bool &reverse = false);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~AxisJoystick();

		float GetAmount() const override;

		JoystickPort GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		std::vector<int> GetAxes() const { return m_axes; }

		void SetAxes(const std::vector<int> &axes) { m_axes = axes; }
	};
}
