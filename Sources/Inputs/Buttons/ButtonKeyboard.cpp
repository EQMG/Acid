#include "ButtonKeyboard.hpp"

namespace acid {
ButtonKeyboard::ButtonKeyboard(Key key) :
	key(key) {
	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (this->key == key) {
			onButton(action, mods);
		}
	}, this);
}

bool ButtonKeyboard::IsDown() const {
	return (Keyboard::Get()->GetKey(key) != InputAction::Release) ^ inverted;
}

Axis::ArgumentDescription ButtonKeyboard::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"key", "int", "The key on the keyboard being checked"}
	};
}

const Node &operator>>(const Node &node, ButtonKeyboard &buttonKeyboard) {
	node["inverted"].Get(buttonKeyboard.inverted);
	node["key"].Get(buttonKeyboard.key);
	return node;
}

Node &operator<<(Node &node, const ButtonKeyboard &buttonKeyboard) {
	node["inverted"].Set(buttonKeyboard.inverted);
	node["key"].Set(buttonKeyboard.key);
	return node;
}
}
