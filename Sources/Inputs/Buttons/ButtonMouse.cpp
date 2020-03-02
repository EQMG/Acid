#include "ButtonMouse.hpp"

namespace acid {
ButtonMouse::ButtonMouse(MouseButton button) :
	button(button) {
	Mouse::Get()->OnButton().Add([this](MouseButton button, InputAction action, BitMask<InputMod> mods) {
		if (this->button == button) {
			onButton(action, mods);
		}
	}, this);
}

bool ButtonMouse::IsDown() const {
	return (Mouse::Get()->GetButton(button) != InputAction::Release) ^ inverted;
}

Axis::ArgumentDescription ButtonMouse::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"key", "int", "The mouse button on the mouse being checked"}
	};
}

const Node &operator>>(const Node &node, ButtonMouse &buttonMouse) {
	node["inverted"].Get(buttonMouse.inverted);
	node["button"].Get(buttonMouse.button);
	return node;
}

Node &operator<<(Node &node, const ButtonMouse &buttonMouse) {
	node["inverted"].Set(buttonMouse.inverted);
	node["button"].Set(buttonMouse.button);
	return node;
}
}
