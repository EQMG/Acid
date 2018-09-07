#include "ButtonMouse.hpp"

namespace acid
{
	ButtonMouse::ButtonMouse(const std::vector<MouseButton> &buttons) :
		m_buttons(buttons),
		m_wasDown(false)
	{
	}

	ButtonMouse::~ButtonMouse()
	{
	}

	bool ButtonMouse::IsDown() const
	{
		for (auto &button : m_buttons)
		{
			if (Mouse::Get()->GetButton(button))
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonMouse::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
