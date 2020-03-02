#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputRadio : public UiObject {
public:
	enum class Type {
		Filled, X, Dot, Check
	};

	UiInputRadio();

	void UpdateObject() override;

	const std::string &GetTitle() const { return title.GetString(); }
	void SetTitle(const std::string &string) { title.SetString(string); }

	bool GetValue() const { return value; }
	void SetValue(bool value);

	const Type &GetType() const { return type; }
	void SetType(const Type &type);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnValue() { return onValue; }

private:
	void UpdateValue();

	Gui background;
	Gui fill;
	Text title;

	bool value = false;
	Type type = Type::Filled;

	Delegate<void(bool)> onValue;
};

class ACID_EXPORT UiRadioManager : public virtual Observer {
public:
	UiRadioManager() = default;

	const UiInputRadio::Type &GetMarkType() const { return type; }

	void SetMarkType(const UiInputRadio::Type &type) {
		this->type = type;

		for (auto &input : inputs) {
			input->SetType(type);
		}
	}

	const std::vector<UiInputRadio *> &GetInputs() const { return inputs; }

	void AddInputs(const std::vector<UiInputRadio *> &inputs) {
		for (auto &input : inputs) {
			AddInput(input);
		}
	}

	void AddInput(UiInputRadio *input) {
		inputs.emplace_back(input);
		input->SetType(type);
		input->OnValue().Add([this, input](bool value) {
			if (!multiple) {
				for (auto &input2 : inputs) {
					if (input2 != input) {
						input2->SetValue(false);
					}
				}
			}
		}, this);
	}

	void RemoveInput(UiInputRadio *input) {
		inputs.erase(std::remove(inputs.begin(), inputs.end(), input), inputs.end());
	}

	void ClearInputs() { inputs.clear(); }

private:
	UiInputRadio::Type type = UiInputRadio::Type::X;
	bool multiple = false;
	std::vector<UiInputRadio *> inputs;
};
}
