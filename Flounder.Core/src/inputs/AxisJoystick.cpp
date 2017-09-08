#include "axisjoystick.hpp"

namespace Flounder
{
	AxisJoystick::AxisJoystick(const int &joystick, const int n_args, ...) :
		IAxis(),
		m_joystick(joystick),
		m_count(n_args),
		m_axes(new int[n_args])
	{
		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_axes[i] = va_arg(ap, int);
		}

		va_end(ap);
	}

	AxisJoystick::~AxisJoystick()
	{
		delete m_axes;
	}

	float AxisJoystick::GetAmount() const
	{
		if (Joysticks::Get() == nullptr || !Joysticks::Get()->IsConnected(m_joystick))
		{
			return 0.0f;
		}

		float result = 0.0f;

		for (int i = 0; i < m_count; i++)
		{
			result += Joysticks::Get()->GetAxis(m_joystick, m_axes[i]);
		}

		return Maths::Clamp(result, -1.0f, 1.0f);
	}
}
