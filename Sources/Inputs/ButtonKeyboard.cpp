#include "ButtonKeyboard.hpp"

namespace acid
{
	ButtonKeyboard::ButtonKeyboard(const Key &key) :
		m_key(key),
		m_wasDown(false)
	{
		Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, BitMask<InputMod> mods)
		{
			if (key == m_key)
			{
				m_onButton(action, mods);
			}
		};
	}

	bool ButtonKeyboard::IsDown() const
	{
		return Keyboard::Get()->GetKey(m_key) != InputAction::Release;
	}

	bool ButtonKeyboard::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
