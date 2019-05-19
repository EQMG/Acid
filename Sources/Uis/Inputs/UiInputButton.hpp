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
	static const Vector2f Size;
	static const Vector2f Padding;
	static const float FontSize;
	static const Time SlideTime;

	static const Colour TitleColour;
	static const Colour ValueColour;

	static const Colour BackgroundColour;
	static const Colour PrimaryColour;
	static const Colour SelectedColour;

	UiInputButton(UiObject *parent, const std::string &string, const UiTransform &rectangle = UiTransform(Vector2f(0.0f, 0.0f), UiAnchor::Centre, UiAspect::Position | UiAspect::Size));

	void UpdateObject() override;

	const std::string &GetString() const { return m_text.GetString(); }

	void SetString(const std::string &string) { m_text.SetString(string); }

private:
	Gui m_background;
	Text m_text;
};
}
