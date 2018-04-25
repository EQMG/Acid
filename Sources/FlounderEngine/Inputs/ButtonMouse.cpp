#include "ButtonMouse.hpp"

#include "Mouse.hpp"

namespace fl
{
	ButtonMouse::ButtonMouse(const std::vector<int> &buttons) :
		IButton(),
		m_buttons(std::vector<int>(buttons)),
		m_wasDown(false)
	{
	}

	ButtonMouse::~ButtonMouse()
	{
	}

	bool ButtonMouse::IsDown() const
	{
		if (Mouse::Get() == nullptr)
		{
			return false;
		}

		for (auto button : m_buttons)
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
		const bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
