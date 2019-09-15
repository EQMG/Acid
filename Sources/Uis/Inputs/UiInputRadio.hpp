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

	UiInputRadio(UiObject *parent, const std::string &string, const Type &type = Type::Filled, bool value = false,
		const UiTransform &transform = {UiInputButton::Size});

	void UpdateObject() override;

	const std::string &GetString() const { return m_text.GetString(); }
	void SetString(const std::string &string) { m_text.SetString(string); }

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
	Text m_text;

	bool m_value;
	Type m_type;

	Delegate<void(bool)> m_onValue;
};

class ACID_EXPORT UiRadioManager : public Observer {
public:
	explicit UiRadioManager(const UiInputRadio::Type &type = UiInputRadio::Type::X, bool multiple = false, const std::vector<UiInputRadio *> &inputs = {}) :
		m_type(type),
		m_multiple(multiple),
		m_inputs(inputs) {
		for (auto &input : inputs) {
			input->SetType(type);
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
	}

	const UiInputRadio::Type &GetMarkType() const { return m_type; }
	
	void SetMarkType(const UiInputRadio::Type &type) {
		m_type = type;

		for (auto &input : m_inputs) {
			input->SetType(m_type);
		}
	}

	const std::vector<UiInputRadio *> &GetInputs() const { return m_inputs; }

	void AddInput(UiInputRadio *input) {
		m_inputs.emplace_back(input);
	}

	void RemoveInput(UiInputRadio *input) {
		m_inputs.erase(std::remove(m_inputs.begin(), m_inputs.end(), input), m_inputs.end());
	}

	void ClearInputs() { m_inputs.clear(); }

private:
	UiInputRadio::Type m_type;
	bool m_multiple;
	std::vector<UiInputRadio *> m_inputs;
};
}
