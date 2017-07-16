#pragma once

#include <stdarg.h>

#include "../devices/joysticks.h"
#include "../maths/maths.h"

#include "iaxis.h"

namespace flounder {
	/// <summary>
	/// Axis from a joystick.
	/// </summary>
	class axisjoystick : public iaxis
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
		axisjoystick(const int &joystick, const int n_args, ...)
		{
			m_joystick = joystick;
			m_count = n_args;
			m_axes = new int[n_args];

			va_list ap;
			va_start(ap, n_args);
			
			for (int i = 0; i < n_args; i++) 
			{
				m_axes[i] = va_arg(ap, int);
			}

			va_end(ap);
		}

		/// <summary>
		/// Deconstructor for the axis joystick.
		/// </summary>
		~axisjoystick() 
		{
			delete m_axes;
		}

		float getAmount() override 
		{
			if (m_count == 0 || !joysticks::get()->isConnected(m_joystick))
			{
				return 0.0f;
			}

			float result = 0.0f;

			for (int i = 0; i < m_count; i++)
			{
				result += joysticks::get()->getAxis(m_joystick, m_axes[i]);
			}

			return maths::clamp(result, -1.0f, 1.0f);
		}
	};
}
