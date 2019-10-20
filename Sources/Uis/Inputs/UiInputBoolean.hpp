#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputBoolean : public UiObject {
public:
	UiInputBoolean(UiObject *parent, const std::string &title, bool value, const UiTransform &transform = {UiInputButton::Size});

	void UpdateObject() override;

	std::string GetTitle() const { return String::ConvertUtf8(m_textTitle.GetString()); }
	void SetTitle(std::string_view string) { m_textTitle.SetString(string); }

	bool GetValue() const { return m_value; }
	void SetValue(bool value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnValue() { return m_onValue; }

private:
	void UpdateValue();

	Gui m_slider;
	Gui m_background;
	Text m_textTitle;
	Text m_textValue;

	bool m_value;

	Delegate<void(bool)> m_onValue;
};
}
