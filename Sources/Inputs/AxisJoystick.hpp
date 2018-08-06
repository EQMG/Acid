#pragma once

#include <vector>
#include "IAxis.hpp"
#include "Joysticks.hpp"

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
		std::vector<uint32_t> m_axes;
		bool m_reverse;
	public:
		/// <summary>
		/// Creates a new axis joystick.
		/// </summary>
		/// <param name="joystick"> The joystick port. </param>
		/// <param name="axes"> The axes on the joystick being checked. </param>
		/// <param name="reverse"> If the axis direction should be inverted. </param>
		AxisJoystick(const JoystickPort &joystick, const std::vector<uint32_t> &axes, const bool &reverse = false);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~AxisJoystick();

		float GetAmount() const override;

		JoystickPort GetJoystick() const { return m_joystick; }

		void SetJoystick(const JoystickPort &joystickPort) { m_joystick = joystickPort; }

		std::vector<uint32_t> GetAxes() const { return m_axes; }

		void SetAxes(const std::vector<uint32_t> &axes) { m_axes = axes; }
	};
}
