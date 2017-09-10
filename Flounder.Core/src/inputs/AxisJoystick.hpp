#pragma once

#include <stdarg.h>

#include "../devices/Joysticks.hpp"
#include "../maths/Maths.hpp"

#include "iaxis.hpp"

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
		int m_count;
		int *m_axes;
	public:
		/// <summary>
		/// Creates a new axis joystick.
		/// </summary>
		/// <param name="joystick"> The joystick. Should be one of the GLFW.JOYSTICK values. </param>
		/// <param name="n_args"> The number axes of joystick axes being checked. </param>
		/// <param name="..."> The axes on the joystick being checked. </param>
		AxisJoystick(const int &joystick, const int n_args, ...);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~AxisJoystick();

		float GetAmount() const override;
	};
}
