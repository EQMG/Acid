#include "AxisButton.hpp"

namespace acid
{
AxisButton::AxisButton(Button *negative, Button *positive) :
	m_negative(negative),
	m_positive(positive)
{
	m_negative->OnButton() += [this](InputAction action, BitMask<InputMod> mods)
	{
		m_onAxis(GetAmount());
	};
	m_positive->OnButton() += [this](InputAction action, BitMask<InputMod> mods)
	{
		m_onAxis(GetAmount());
	};
}

float AxisButton::GetAmount() const
{
	auto amount = 0.0f;

	if (m_positive->IsDown())
	{
		amount += 1.0f;
	}

	if (m_negative->IsDown())
	{
		amount -= 1.0f;
	}

	return amount;
}
}
