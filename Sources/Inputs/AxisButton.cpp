#include "AxisButton.hpp"

namespace acid
{
	AxisButton::AxisButton(const std::shared_ptr<IButton> &negative, const std::shared_ptr<IButton> &positive) :
		m_negative(negative),
		m_positive(positive)
	{
	}

	AxisButton::~AxisButton()
	{
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
