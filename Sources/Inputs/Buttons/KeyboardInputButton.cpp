#include "KeyboardInputButton.hpp"

namespace acid {
KeyboardInputButton::KeyboardInputButton(Key key) :
	key(key) {
	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (this->key == key) {
			onButton(action, mods);
		}
	}, this);
}

bool KeyboardInputButton::IsDown() const {
	return (Keyboard::Get()->GetKey(key) != InputAction::Release) ^ inverted;
}

InputAxis::ArgumentDescription KeyboardInputButton::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"key", "int", "The key on the keyboard being checked"}
	};
}

const Node &operator>>(const Node &node, KeyboardInputButton &inputButton) {
	node["inverted"].Get(inputButton.inverted);
	node["key"].Get(inputButton.key);
	return node;
}

Node &operator<<(Node &node, const KeyboardInputButton &inputButton) {
	node["inverted"].Set(inputButton.inverted);
	node["key"].Set(inputButton.key);
	return node;
}
}
