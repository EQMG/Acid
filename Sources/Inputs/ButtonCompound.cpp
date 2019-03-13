#include "ButtonCompound.hpp"

namespace acid
{
ButtonCompound::ButtonCompound(const std::vector<IButton*>& buttons, const bool& useAnd) : m_useAnd(useAnd), m_wasDown(false)
{
	for(const auto& button : buttons)
		{
			m_buttons.emplace_back(button);
		}
}

bool ButtonCompound::IsDown() const
{
	for(const auto& button : m_buttons)
		{
			if(m_useAnd && !button->IsDown())
				{
					return false;
				}
			if(!m_useAnd && button->IsDown())
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
