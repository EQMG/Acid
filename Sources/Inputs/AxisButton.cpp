#include "AxisButton.hpp"

namespace acid
{
AxisButton::AxisButton(Button *negative, Button *positive) :
	m_negative(negative),
	m_positive(positive)
{
	m_negative->OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		m_onAxis(GetAmount());
	}, this);
	m_positive->OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		m_onAxis(GetAmount());
	}, this);
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
