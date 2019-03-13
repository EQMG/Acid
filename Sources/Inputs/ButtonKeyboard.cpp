#include "ButtonKeyboard.hpp"

namespace acid
{
ButtonKeyboard::ButtonKeyboard(const Key& key) : m_key(key), m_wasDown(false) {}

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
