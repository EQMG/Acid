#include "ButtonMouse.hpp"

namespace acid
{
ButtonMouse::ButtonMouse(const MouseButton &button) :
	m_button(button),
	m_wasDown(false)
{
	Mouse::Get()->GetOnButton() += [this](MouseButton button, InputAction action, BitMask<InputMod> mods)
	{
		if (button == m_button)
		{
			m_onButton(action, mods);
		}
	};
}

bool ButtonMouse::IsDown() const
{
	return Mouse::Get()->GetButton(m_button) != InputAction::Release;
}

bool ButtonMouse::WasDown()
{
	bool stillDown = m_wasDown && IsDown();
	m_wasDown = IsDown();
	return m_wasDown == !stillDown;
}
}
