#include "ButtonCompound.hpp"

namespace acid
{
	ButtonCompound::ButtonCompound(const std::vector<IButton *> &buttons) :
		m_buttons(std::vector<std::unique_ptr<IButton>>()),
		m_wasDown(false)
	{
		for (auto &button : buttons)
		{
			m_buttons.emplace_back(button);
		}
	}

	bool ButtonCompound::IsDown() const
	{
		for (auto &button : m_buttons)
		{
			if (button->IsDown())
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
