#include "ButtonMouse.hpp"

namespace acid {
bool ButtonMouse::registered = Register("buttonMouse");

ButtonMouse::ButtonMouse(MouseButton button) :
	m_button(button) {
	Mouse::Get()->OnButton().Add([this](MouseButton button, InputAction action, BitMask<InputMod> mods) {
		if (button == m_button) {
			m_onButton(action, mods);
		}
	}, this);
}

bool ButtonMouse::IsDown() const {
	return (Mouse::Get()->GetButton(m_button) != InputAction::Release) ^ m_inverted;
}

Axis::ArgumentDescription ButtonMouse::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"key", "int", "The mouse button on the mouse being checked"}
	};
}

const Node &operator>>(const Node &node, ButtonMouse &buttonMouse) {
	node["inverted"].Get(buttonMouse.m_inverted);
	node["button"].Get(buttonMouse.m_button);
	return node;
}

Node &operator<<(Node &node, const ButtonMouse &buttonMouse) {
	node["inverted"].Set(buttonMouse.m_inverted);
	node["button"].Set(buttonMouse.m_button);
	return node;
}
}
