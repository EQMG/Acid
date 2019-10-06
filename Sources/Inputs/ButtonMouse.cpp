#include "ButtonMouse.hpp"

namespace acid {
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
}
