#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"

namespace acid {
class ACID_EXPORT UiButtonInput : public UiObject {
public:
	UiButtonInput();

	void UpdateObject() override;

	const std::string &GetTitle() const { return title.GetString(); }
	void SetTitle(const std::string &string) { title.SetString(string); }
	
	constexpr static Vector2i Size = {175, 28};
	constexpr static Vector2i Padding = {5, 5};
	constexpr static float FontSize = 13.0f;
	constexpr static Time SlideTime = 0.07s;

	constexpr static Colour ValueColour = 0xFEFCFE;
	constexpr static Colour TitleColour = 0x9C9A9C;

	constexpr static Colour BackgroundColour = 0x282729;
	constexpr static Colour PrimaryColour = 0x121113;
	constexpr static Colour SelectedColour = 0xFEA62A;
	constexpr static Colour ButtonColour = 0x3C3B3C;

private:
	Gui background;
	Text title;
};
}
