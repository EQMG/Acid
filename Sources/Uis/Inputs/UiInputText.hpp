#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "Inputs/InputDelay.hpp"
#include "UiInputButton.hpp"

namespace acid
{
class ACID_EXPORT UiInputText :
	public UiObject
{
public:
	UiInputText(UiObject *parent, const std::string &title, std::string value, const int32_t &maxLength,
		const UiTransform &transform = UiTransform(UiInputButton::Size));

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_textTitle.GetString(); }

	void SetTitle(const std::string &title) { m_textTitle.SetString(title); }

	const std::string &GetValue() const { return m_value; }

	void SetValue(const std::string &value);

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(std::string)> &OnValue() { return m_onValue; }

private:
	void SetUpdating(const bool &updating);

	Gui m_background;
	Text m_textTitle;
	Text m_textValue;

	std::string m_value;
	int32_t m_maxLength;

	InputDelay m_inputDelay;
	int32_t m_lastKey;

	bool m_updating;
	bool m_mouseOver;

	Delegate<void(std::string)> m_onValue;
};
}
