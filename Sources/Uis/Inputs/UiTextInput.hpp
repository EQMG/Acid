#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "Inputs/InputDelay.hpp"
#include "UiButtonInput.hpp"

namespace acid {
class ACID_EXPORT UiTextInput : public UiObject {
public:
	UiTextInput();

	void UpdateObject() override;

	const std::string &GetTitle() const { return textTitle.GetString(); }
	void SetTitle(const std::string &string) { textTitle.SetString(string); }

	const std::string &GetValue() const { return value; }
	void SetValue(const std::string &value);

	int32_t GetMaxLength() const { return maxLength; }
	void SetMaxLength(int32_t maxLength) { this->maxLength = maxLength; }

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(std::string)> &OnValue() { return onValue; }

private:
	void SetUpdating(bool updating);

	Gui background;
	Text textTitle;
	Text textValue;

	std::string value;
	int32_t maxLength = 16;

	InputDelay inputDelay;
	int32_t lastKey = 0;

	bool updating = false;
	bool mouseOver = false;

	Delegate<void(std::string)> onValue;
};
}
