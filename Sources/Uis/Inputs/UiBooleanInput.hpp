#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiButtonInput.hpp"

namespace acid {
class ACID_EXPORT UiBooleanInput : public UiObject {
public:
	UiBooleanInput();

	void UpdateObject() override;

	const std::string &GetTitle() const { return textTitle.GetString(); }
	void SetTitle(const std::string &string) { textTitle.SetString(string); }

	bool GetValue() const { return value; }
	void SetValue(bool value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	rocket::signal<void(bool)> &OnValue() { return onValue; }

private:
	void UpdateValue();

	Gui slider;
	Gui background;
	Text textTitle;
	Text textValue;

	bool value = false;

	rocket::signal<void(bool)> onValue;
};
}
