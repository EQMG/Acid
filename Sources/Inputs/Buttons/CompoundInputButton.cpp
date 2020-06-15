#include "CompoundInputButton.hpp"

namespace acid {
CompoundInputButton::CompoundInputButton(std::vector<std::unique_ptr<InputButton>> &&buttons, bool useAnd) :
	buttons(std::move(buttons)),
	useAnd(useAnd) {
	ConnectButtons();
}

bool CompoundInputButton::IsDown() const {
	for (const auto &button : buttons) {
		if (useAnd && !button->IsDown())
			return false ^ inverted;

		if (!useAnd && button->IsDown())
			return true ^ inverted;
	}

	return useAnd ^ inverted;
}

InputAxis::ArgumentDescription CompoundInputButton::GetArgumentDescription() const {
	return {
		{"inverted", "bool", "If the down reading will be inverted"},
		{"buttons", "button[]", "The buttons that will be combined into a compound button"},
		{"useAnd", "bool", "If must be down for a down reading, or just one"}
	};
}

InputButton *CompoundInputButton::AddButton(std::unique_ptr<InputButton> &&button) {
	auto &result = buttons.emplace_back(std::move(button));
	ConnectButton(result);
	return result.get();
}

void CompoundInputButton::RemoveButton(InputButton *button) {
	//button->OnButton().RemoveObservers(this);
	buttons.erase(std::remove_if(buttons.begin(), buttons.end(), [button](std::unique_ptr<InputButton> &b) {
		return b.get() == button;
	}), buttons.end());
}

void CompoundInputButton::ConnectButton(std::unique_ptr<InputButton> &button) {
	button->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
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
	});
}

void CompoundInputButton::ConnectButtons() {
	for (auto &button : buttons)
		ConnectButton(button);
}

const Node &operator>>(const Node &node, CompoundInputButton &inputButton) {
	node["inverted"].Get(inputButton.inverted);
	node["buttons"].Get(inputButton.buttons);
	node["useAnd"].Get(inputButton.useAnd);
	inputButton.ConnectButtons();
	return node;
}

Node &operator<<(Node &node, const CompoundInputButton &inputButton) {
	node["inverted"].Set(inputButton.inverted);
	node["buttons"].Set(inputButton.buttons);
	node["useAnd"].Set(inputButton.useAnd);
	return node;
}
}
