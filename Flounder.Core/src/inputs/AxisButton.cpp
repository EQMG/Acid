#include "axisbutton.hpp"

namespace Flounder
{
	axisbutton::axisbutton(ibutton *negative, ibutton *positive) :
		iaxis(),
		m_negative(negative),
		m_positive(positive)
	{
	}

	axisbutton::~axisbutton()
	{
		delete m_negative;
		delete m_positive;
	}

	float axisbutton::getAmount() const
	{
		float result = 0.0f;

		if (m_positive->isDown())
		{
			result += 1.0f;
		}

		if (m_negative->isDown())
		{
			result -= 1.0f;
		}

		return result;
	}
}
