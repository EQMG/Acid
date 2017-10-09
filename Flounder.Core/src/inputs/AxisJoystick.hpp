#pragma once

#include <vector>
#include "IAxis.hpp"

namespace Flounder
{
	/// <summary>
	/// Axis from a joystick.
	/// </summary>
	class AxisJoystick :
		public IAxis
	{
	private:
		int m_joystick;
		std::vector<int> m_axes;
	public:
		/// <summary>
		/// Creates a new axis joystick.
		/// </summary>
		/// <param name="joystick"> The joystick. Should be one of the GLFW_JOYSTICK values. </param>
		/// <param name="axes"> The axes on the joystick being checked. </param>
		AxisJoystick(const int &joystick, const std::vector<int> &axes);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~AxisJoystick();

		float GetAmount() const override;
	};
}
