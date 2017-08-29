#pragma once

#include <stdarg.h>

#include "../devices/joysticks.hpp"
#include "../maths/maths.hpp"

#include "iaxis.hpp"
#include "ibutton.hpp"

namespace flounder
{
	/// <summary>
	/// Axis composed of two buttons.
	/// </summary>
	class axisbutton :
		public iaxis
	{
	private:
		ibutton *m_negative;
		ibutton *m_positive;
	public:
		/// <summary>
		/// Creates a new axis button.
		/// </summary>
		/// <param name="negative"> When this button is down, the axis is negative. </param>
		/// <param name="positive"> When this button is down, the axis is positive. </param>
		axisbutton(ibutton *negative, ibutton *positive);

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~axisbutton();

		float getAmount() const override;
	};
}
