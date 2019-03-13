#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputBoolean : public UiObject
{
  public:
	UiInputBoolean(UiObject* parent, const std::string& title, const bool& value, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	const std::string& GetTitle() const
	{
		return m_title;
	}

	void SetTitle(const std::string& title);

	const bool& GetValue() const
	{
		return m_value;
	}

	void SetValue(const bool& value);

	Delegate<void(UiInputBoolean*, bool)>& GetOnChange()
	{
		return m_onChange;
	}

  private:
	void UpdateValueText();

	Gui m_slider;
	Gui m_background;
	Text m_textTitle;
	Text m_textValue;
	Sound m_soundClick;

	std::string m_title;
	bool m_value;

	bool m_mouseOver;
	Delegate<void(UiInputBoolean*, bool)> m_onChange;
};
}
