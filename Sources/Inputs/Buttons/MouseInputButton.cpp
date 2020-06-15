#include "MouseInputButton.hpp"

namespace acid {
MouseInputButton::MouseInputButton(MouseButton button) :
	button(button) {
	Mouse::Get()->OnButton().connect(this, [this](MouseButton button, InputAction action, bitmask::bitmask<InputMod> mods) {
		if (this->button == button) {
			onButton(action, mods);
		}
	});
}

bool MouseInputButton::IsDown() const {
	return (Mouse::Get()->GetButton(button) != InputAction::Release) ^ inverted;
}

InputAxis::ArgumentDescription MouseInputButton::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"key", "int", "The mouse button on the mouse being checked"}
	};
}

const Node &operator>>(const Node &node, MouseInputButton &inputButton) {
	node["inverted"].Get(inputButton.inverted);
	node["button"].Get(inputButton.button);
	return node;
}

Node &operator<<(Node &node, const MouseInputButton &inputButton) {
	node["inverted"].Set(inputButton.inverted);
	node["button"].Set(inputButton.button);
	return node;
}
}
