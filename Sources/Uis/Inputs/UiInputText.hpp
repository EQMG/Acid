#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Inputs/InputDelay.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputText : public UiObject
{
  public:
	UiInputText(UiObject* parent, const std::string& title, const std::string& value, const int32_t& maxLength, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	void SetSelected(const bool& selected);

	const std::string& GetTitle() const
	{
		return m_title;
	}

	void SetTitle(const std::string& title);

	const std::string& GetValue() const
	{
		return m_value;
	}

	void SetValue(const std::string& value);

	Delegate<void(UiInputText*, std::string)>& GetOnType()
	{
		return m_onType;
	}

  private:
	Gui m_background;
	Text m_textTitle;
	Text m_textValue;
	Sound m_soundClick;

	std::string m_title;
	std::string m_value;
	int32_t m_maxLength;

	InputDelay m_inputDelay;
	int32_t m_lastKey;

	bool m_selected;
	bool m_mouseOver;

	Delegate<void(UiInputText*, std::string)> m_onType;
};
}
