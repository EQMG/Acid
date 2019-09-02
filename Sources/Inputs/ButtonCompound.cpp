#include "ButtonCompound.hpp"

namespace acid
{
/*ButtonCompound::ButtonCompound(std::vector<std::unique_ptr<Button>> &&buttons, const bool &useAnd) :
	m_buttons{std::move(buttons)},
	m_useAnd{useAnd}
{
	ConnectButtons();
}*/

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

void ButtonCompound::ConnectButtons()
{
	for (auto &button : m_buttons)
	{
		button->OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
		{
			auto isDown = IsDown();

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
		}, this);
	}
}
}
