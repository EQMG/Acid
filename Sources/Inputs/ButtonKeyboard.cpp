#include "ButtonKeyboard.hpp"

namespace acid {
ButtonKeyboard::ButtonKeyboard(const Key &key) :
	m_key(key) {
	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (key == m_key) {
			m_onButton(action, mods);
		}
	}, this);
}

bool ButtonKeyboard::IsDown() const {
	return Keyboard::Get()->GetKey(m_key) != InputAction::Release;
}
}
