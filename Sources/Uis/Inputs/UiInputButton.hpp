#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid {
class ACID_EXPORT UiInputButton : public UiObject {
public:
	UiInputButton();

	void UpdateObject() override;

	const std::string &GetTitle() const { return m_title.GetString(); }
	void SetTitle(const std::string &string) { m_title.SetString(string); }
	
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
	Text m_title;
};
}
