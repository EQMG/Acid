#include "ButtonCompound.hpp"

namespace Flounder
{
	ButtonCompound::ButtonCompound(const std::vector<IButton*> &buttons) :
		IButton(),
		m_buttons(std::vector<IButton*>(buttons)),
		m_wasDown(false)
	{
	}

	ButtonCompound::~ButtonCompound()
	{
	}

	bool ButtonCompound::IsDown() const
	{
		for (auto button : m_buttons)
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
		const bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
