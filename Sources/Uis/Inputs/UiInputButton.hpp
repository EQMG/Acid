#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputButton :
	public UiObject
{
public:
	UiInputButton(UiObject *parent, const std::string &string, const UiTransform &transform = UiTransform(Size, Vector2i(), UiAnchor::Centre));

	void UpdateObject() override;

	const std::string &GetString() const { return m_text.GetString(); }

	void SetString(const std::string &string) { m_text.SetString(string); }

	static const Vector2i Size;
	static const uint32_t Padding;
	static const uint32_t FontSize;
	static const Time SlideTime;

	static const Colour TitleColour;
	static const Colour ValueColour;

	static const Colour BackgroundColour;
	static const Colour PrimaryColour;
	static const Colour SelectedColour;

private:
	Gui m_background;
	Text m_text;
};
}
