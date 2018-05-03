#pragma once

#include <vector>
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
		unsigned int m_joystick;
		std::vector<int> m_axes;
		bool m_reverse;
	public:
		/// <summary>
		/// Creates a new axis joystick.
		/// </summary>
		/// <param name="joystick"> The joystick. Should be one of the GLFW_JOYSTICK values. </param>
		/// <param name="axes"> The axes on the joystick being checked. </param>
		/// <param name="reverse"> If the axis direction should be inverted. </param>
		AxisJoystick(const unsigned int &joystick, const std::vector<int> &axes, const bool &reverse = false);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~AxisJoystick();

		float GetAmount() const override;
	};
}
