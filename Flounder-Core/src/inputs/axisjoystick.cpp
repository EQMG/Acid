#include "axisjoystick.h"

namespace flounder
{
	axisjoystick::axisjoystick(const int &joystick, const int n_args, ...) :
		iaxis()
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

	axisjoystick::~axisjoystick()
	{
		delete m_axes;
	}

	float axisjoystick::getAmount() const
	{
		if (!joysticks::get()->isConnected(m_joystick))
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
}
