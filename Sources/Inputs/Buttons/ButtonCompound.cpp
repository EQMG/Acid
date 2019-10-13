#include "ButtonCompound.hpp"

namespace acid {
bool ButtonCompound::registered = Register("buttonCompound");

ButtonCompound::ButtonCompound(std::vector<std::unique_ptr<Button>> &&buttons, bool useAnd) :
	m_buttons(std::move(buttons)),
	m_useAnd(useAnd) {
	ConnectButtons();
}

bool ButtonCompound::IsDown() const {
	for (const auto &button : m_buttons) {
		if (m_useAnd && !button->IsDown())
			return false ^ m_inverted;

		if (!m_useAnd && button->IsDown())
			return true ^ m_inverted;
	}

	return m_useAnd ^ m_inverted;
}

Axis::ArgumentDescription ButtonCompound::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"buttons", "button[]", "The buttons that will be combined into a compound button"},
		{"useAnd", "bool", "If must be down for a down reading, or just one"}
	};
}

void ButtonCompound::ConnectButtons() {
	for (auto &button : m_buttons) {
		button->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
			auto isDown = IsDown();

			if (!m_lastDown && isDown) {
				m_lastDown = true;
				m_onButton(InputAction::Press, 0);
			} else if (m_lastDown && !isDown) {
				m_lastDown = false;
				m_onButton(InputAction::Release, 0);
			} else if (m_lastDown && isDown) {
				// TODO: This will be sent for every button, only count one per cycle.
				m_onButton(InputAction::Repeat, 0);
			}
		}, this);
	}
}

const Node &operator>>(const Node &node, ButtonCompound &buttonCompound) {
	node["inverted"].Get(buttonCompound.m_inverted);
	node["buttons"].Get(buttonCompound.m_buttons);
	node["useAnd"].Get(buttonCompound.m_useAnd);
	return node;
}

Node &operator<<(Node &node, const ButtonCompound &buttonCompound) {
	node["inverted"].Set(buttonCompound.m_inverted);
	node["buttons"].Set(buttonCompound.m_buttons);
	node["useAnd"].Set(buttonCompound.m_useAnd);
	return node;
}
}
