#include "ButtonKeyboard.hpp"

namespace fl
{
	ButtonKeyboard::ButtonKeyboard(const std::vector<Key> &keys) :
		IButton(),
		m_keys(std::vector<Key>(keys)),
		m_wasDown(false)
	{
	}

	ButtonKeyboard::~ButtonKeyboard()
	{
	}

	bool ButtonKeyboard::IsDown() const
	{
		if (Keyboard::Get() == nullptr)
		{
			return false;
		}

		for (auto &key : m_keys)
		{
			if (Keyboard::Get()->GetKey(key))
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonKeyboard::WasDown()
	{
		const bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
