#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid
{
class ACID_EXPORT UiInputBoolean :
	public UiObject
{
public:
	UiInputBoolean(UiObject *parent, const std::string &title, const bool &value, const UiTransform &transform = UiTransform(UiInputButton::Size));

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_textTitle.GetString(); }

	void SetTitle(const std::string &title) { m_textTitle.SetString(title); }

	const bool &GetValue() const { return m_value; }

	void SetValue(const bool &value);

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
