#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputBoolean :
	public UiObject
{
public:
	UiInputBoolean(UiObject *parent, const std::string &title, const bool &value,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_title; }

	void SetTitle(const std::string &title);

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
	Sound m_soundClick;

	std::string m_title;
	bool m_value;

	bool m_mouseOver;
	Delegate<void(bool)> m_onValue;
};
}
