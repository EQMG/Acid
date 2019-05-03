#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputDropdown :
	public UiObject
{
public:
	UiInputDropdown(UiObject *parent, const std::string &title, const uint32_t &value, const std::vector<std::string> &options,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_textTitle.GetString(); }

	void SetTitle(const std::string &title) { m_textTitle.SetString(title); }

	const uint32_t &GetValue() const { return m_value; }

	void SetValue(const uint32_t &value);

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
