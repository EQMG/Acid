#include "ButtonCompound.hpp"

namespace acid
{
ButtonCompound::ButtonCompound(const std::vector<IButton *> &buttons, const bool &useAnd) :
	m_useAnd(useAnd),
	m_wasDown(false)
{
	for (const auto &button : buttons)
	{
		button->GetOnButton() += [this](InputAction action, BitMask<InputMod> mods)
		{
			if (IsDown() == !(m_wasDown && IsDown()))
			{
				m_onButton(InputAction::Press, 0);
			}
			else if (!IsDown())
			{
				m_onButton(InputAction::Release, 0);
			}
			else
			{
				m_onButton(InputAction::Repeat, 0);
			}
		};
		m_buttons.emplace_back(button);
	}
}

bool ButtonCompound::IsDown() const
{
	for (const auto &button : m_buttons)
	{
		if (m_useAnd && !button->IsDown())
		{
			return false;
		}
		if (!m_useAnd && button->IsDown())
		{
			return true;
		}
	}

	return false;
}

bool ButtonCompound::WasDown()
{
	bool stillDown = m_wasDown && IsDown();
	m_wasDown = IsDown();
	return m_wasDown == !stillDown;
}
}
