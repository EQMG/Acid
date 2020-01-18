#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputDropdown : public UiObject {
public:
	UiInputDropdown(UiObject *parent, const std::string &title, uint32_t value, std::vector<std::string> options,
		const UiTransform &transform = {UiInputButton::Size});

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_textTitle.GetString(); }
	void SetTitle(const std::string &string) { m_textTitle.SetString(string); }

	uint32_t GetValue() const { return m_value; }
	void SetValue(uint32_t value);

	const std::vector<std::string> &GetOptions() const { return m_options; }
	void SetOptions(const std::vector<std::string> &options);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(uint32_t)> &OnValue() { return m_onValue; }

private:
	//Gui m_slider;
	Gui m_background;
	Text m_textTitle;

	uint32_t m_value;
	std::vector<std::string> m_options;

	Delegate<void(uint32_t)> m_onValue;
};
}
