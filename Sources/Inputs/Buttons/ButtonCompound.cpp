#include "ButtonCompound.hpp"

namespace acid {
ButtonCompound::ButtonCompound(std::vector<std::unique_ptr<Button>> &&buttons, bool useAnd) :
	buttons(std::move(buttons)),
	useAnd(useAnd) {
	ConnectButtons();
}

bool ButtonCompound::IsDown() const {
	for (const auto &button : buttons) {
		if (useAnd && !button->IsDown())
			return false ^ inverted;

		if (!useAnd && button->IsDown())
			return true ^ inverted;
	}

	return useAnd ^ inverted;
}

Axis::ArgumentDescription ButtonCompound::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"buttons", "button[]", "The buttons that will be combined into a compound button"},
		{"useAnd", "bool", "If must be down for a down reading, or just one"}
	};
}

Button *ButtonCompound::AddButton(std::unique_ptr<Button> &&button) {
	auto &result = buttons.emplace_back(std::move(button));
	ConnectButton(result);
	return result.get();
}

void ButtonCompound::RemoveButton(Button *button) {
	//button->OnButton().RemoveObservers(this);
	buttons.erase(std::remove_if(buttons.begin(), buttons.end(), [button](std::unique_ptr<Button> &b) {
		return b.get() == button;
	}), buttons.end());
}

void ButtonCompound::ConnectButton(std::unique_ptr<Button> &button) {
	button->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		auto isDown = IsDown();

		if (!lastDown && isDown) {
			lastDown = true;
			onButton(InputAction::Press, 0);
		} else if (lastDown && !isDown) {
			lastDown = false;
			onButton(InputAction::Release, 0);
		} else if (lastDown && isDown) {
			// TODO: This will be sent for every button, only count one per cycle.
			onButton(InputAction::Repeat, 0);
		}
	}, this);
}

void ButtonCompound::ConnectButtons() {
	for (auto &button : buttons)
		ConnectButton(button);
}

const Node &operator>>(const Node &node, ButtonCompound &buttonCompound) {
	node["inverted"].Get(buttonCompound.inverted);
	node["buttons"].Get(buttonCompound.buttons);
	node["useAnd"].Get(buttonCompound.useAnd);
	buttonCompound.ConnectButtons();
	return node;
}

Node &operator<<(Node &node, const ButtonCompound &buttonCompound) {
	node["inverted"].Set(buttonCompound.inverted);
	node["buttons"].Set(buttonCompound.buttons);
	node["useAnd"].Set(buttonCompound.useAnd);
	return node;
}
}
