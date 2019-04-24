#include "ButtonCompound.hpp"

namespace acid
{
ButtonCompound::ButtonCompound(const std::vector<Button *> &buttons, const bool &useAnd) :
	m_useAnd(useAnd),
	m_lastDown(false)
{
	for (const auto &button : buttons)
	{
		button->OnButton() += [this](InputAction action, BitMask<InputMod> mods)
		{
			bool isDown = IsDown();

			if (!m_lastDown && isDown)
			{
				m_lastDown = true;
				m_onButton(InputAction::Press, 0);
			}
			else if (m_lastDown && !isDown)
			{
				m_lastDown = false;
				m_onButton(InputAction::Release, 0);
			}
			else if (m_lastDown && isDown) // TODO: This will be sent for every button, only count one per cycle.
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

	return m_useAnd;
}
}
