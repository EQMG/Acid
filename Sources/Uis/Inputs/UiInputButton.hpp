#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include <functional>

namespace acid
{
class ACID_EXPORT UiInputButton : public UiObject
{
  public:
	static const Vector2 Size;
	static const Vector2 Padding;
	static const float FontSize;
	static const Time SlideTime;

	static const Colour PrimaryColour;
	static const Colour BackgroundColour;
	static const Colour AccentColour;
	static const Colour SelectedColour;

	UiInputButton(UiObject* parent, const std::string& string, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	const std::string& GetString() const
	{
		return m_text.GetString();
	}

	void SetString(const std::string& string)
	{
		m_text.SetString(string);
	}

  private:
	Gui m_background;
	Text m_text;
	Sound m_soundClick;

	bool m_mouseOver;
};
}
