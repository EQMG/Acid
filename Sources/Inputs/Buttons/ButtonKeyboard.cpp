#include "ButtonKeyboard.hpp"

namespace acid {
bool ButtonKeyboard::registered = Register("buttonKeyboard");

ButtonKeyboard::ButtonKeyboard(Key key) :
	m_key(key) {
	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (key == m_key) {
			m_onButton(action, mods);
		}
	}, this);
}

bool ButtonKeyboard::IsDown() const {
	return (Keyboard::Get()->GetKey(m_key) != InputAction::Release) ^ m_inverted;
}

Axis::ArgumentDescription ButtonKeyboard::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"key", "int", "The key on the keyboard being checked"}
	};
}

const Node &operator>>(const Node &node, ButtonKeyboard &buttonKeyboard) {
	node["inverted"].Get(buttonKeyboard.m_inverted);
	node["key"].Get(buttonKeyboard.m_key);
	return node;
}

Node &operator<<(Node &node, const ButtonKeyboard &buttonKeyboard) {
	node["inverted"].Set(buttonKeyboard.m_inverted);
	node["key"].Set(buttonKeyboard.m_key);
	return node;
}
}
