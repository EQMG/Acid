#include "AxisButton.hpp"

namespace fl
{
	AxisButton::AxisButton(IButton *negative, IButton *positive) :
		IAxis(),
		m_negative(negative),
		m_positive(positive)
	{
	}

	AxisButton::~AxisButton()
	{
		delete m_negative;
		delete m_positive;
	}

	float AxisButton::GetAmount() const
	{
		float result = 0.0f;

		if (m_positive->IsDown())
		{
			result += 1.0f;
		}

		if (m_negative->IsDown())
		{
			result -= 1.0f;
		}

		return result;
	}
}
