#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid {
class ACID_EXPORT UiInputButton : public UiObject {
public:
	UiInputButton(UiObject *parent, const std::string &string, const UiTransform &transform = {Size});

	void UpdateObject() override;

	std::string GetString() const { return String::ConvertUtf8(m_text.GetString()); }
	void SetString(std::string_view string) { m_text.SetString(string); }

	static const Vector2i Size;
	static const Vector2i Padding;
	static const float FontSize;
	static const Time SlideTime;

	static const Colour ValueColour;
	static const Colour TitleColour;

	static const Colour BackgroundColour;
	static const Colour PrimaryColour;
	static const Colour SelectedColour;
	static const Colour ButtonColour;

private:
	Gui m_background;
	Text m_text;
};
}
