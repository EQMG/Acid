#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class UiInputButton :
	public UiObject
{
public:
	UiInputButton(UiObject *parent, const std::string &string, const UiTransform &transform = UiTransform(Size));

	void UpdateObject() override;

	const std::string &GetString() const { return m_text.GetString(); }

	void SetString(const std::string &string) { m_text.SetString(string); }

	ACID_EXPORT static const Vector2i Size;
	ACID_EXPORT static const Vector2i Padding;
	ACID_EXPORT static const float FontSize;
	ACID_EXPORT static const Time SlideTime;

	ACID_EXPORT static const Colour ValueColour;
	ACID_EXPORT static const Colour TitleColour;

	ACID_EXPORT static const Colour BackgroundColour;
	ACID_EXPORT static const Colour PrimaryColour;
	ACID_EXPORT static const Colour SelectedColour;
	ACID_EXPORT static const Colour ButtonColour;

private:
	Gui m_background;
	Text m_text;
};
}
