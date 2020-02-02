#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "Inputs/InputDelay.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputText : public UiObject {
public:
	UiInputText();

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_textTitle.GetString(); }
	void SetTitle(const std::string &string) { m_textTitle.SetString(string); }

	const std::string &GetValue() const { return m_value; }
	void SetValue(const std::string &value);

	int32_t GetMaxLength() const { return m_maxLength; }
	void SetMaxLength(int32_t maxLength) { m_maxLength = maxLength; }

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(std::string)> &OnValue() { return m_onValue; }

private:
	void SetUpdating(bool updating);

	Gui m_background;
	Text m_textTitle;
	Text m_textValue;

	std::string m_value;
	int32_t m_maxLength = 16;

	InputDelay m_inputDelay;
	int32_t m_lastKey = 0;

	bool m_updating = false;
	bool m_mouseOver = false;

	Delegate<void(std::string)> m_onValue;
};
}
