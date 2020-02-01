#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputRadio : public UiObject {
public:
	enum class Type {
		Filled,
		X,
		Dot,
		Check
	};

	UiInputRadio();

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_title.GetString(); }
	void SetTitle(const std::string &string) { m_title.SetString(string); }

	bool GetValue() const { return m_value; }
	void SetValue(bool value);

	const Type &GetType() const { return m_type; }
	void SetType(const Type &type);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnValue() { return m_onValue; }

private:
	void UpdateValue();

	Gui m_background;
	Gui m_fill;
	Text m_title;

	bool m_value = false;
	Type m_type = Type::Filled;

	Delegate<void(bool)> m_onValue;
};

class ACID_EXPORT UiRadioManager : public virtual Observer {
public:
	UiRadioManager() = default;

	const UiInputRadio::Type &GetMarkType() const { return m_type; }

	void SetMarkType(const UiInputRadio::Type &type) {
		m_type = type;

		for (auto &input : m_inputs) {
			input->SetType(m_type);
		}
	}

	const std::vector<UiInputRadio *> &GetInputs() const { return m_inputs; }

	void AddInputs(const std::vector<UiInputRadio *> &inputs) {
		for (auto &input : inputs) {
			AddInput(input);
		}
	}

	void AddInput(UiInputRadio *input) {
		m_inputs.emplace_back(input);
		input->SetType(m_type);
		input->OnValue().Add([this, input](bool value) {
			if (!m_multiple) {
				for (auto &input2 : m_inputs) {
					if (input2 != input) {
						input2->SetValue(false);
					}
				}
			}
		}, this);
	}

	void RemoveInput(UiInputRadio *input) {
		m_inputs.erase(std::remove(m_inputs.begin(), m_inputs.end(), input), m_inputs.end());
	}

	void ClearInputs() { m_inputs.clear(); }

private:
	UiInputRadio::Type m_type = UiInputRadio::Type::X;
	bool m_multiple = false;
	std::vector<UiInputRadio *> m_inputs;
};
}
