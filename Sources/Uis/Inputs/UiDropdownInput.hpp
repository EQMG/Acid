#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiButtonInput.hpp"

namespace acid {
class ACID_EXPORT UiDropdownInput : public UiObject {
public:
	UiDropdownInput();

	void UpdateObject() override;

	const std::string &GetTitle() const { return textTitle.GetString(); }
	void SetTitle(const std::string &string) { textTitle.SetString(string); }

	uint32_t GetValue() const { return value; }
	void SetValue(uint32_t value);

	const std::vector<std::string> &GetOptions() const { return options; }
	void SetOptions(const std::vector<std::string> &options);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t)> &OnValue() { return onValue; }

private:
	//Gui slider;
	Gui background;
	Text textTitle;

	uint32_t value;
	std::vector<std::string> options;

	Delegate<void(uint32_t)> onValue;
};
}
