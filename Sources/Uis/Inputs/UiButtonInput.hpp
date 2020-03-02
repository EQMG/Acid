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
	
	static constexpr Vector2i Size = {175, 28};
	static constexpr Vector2i Padding = {5, 5};
	static constexpr float FontSize = 13.0f;
	static constexpr Time SlideTime = 0.07s;

	static constexpr Colour ValueColour = 0xFEFCFE;
	static constexpr Colour TitleColour = 0x9C9A9C;

	static constexpr Colour BackgroundColour = 0x282729;
	static constexpr Colour PrimaryColour = 0x121113;
	static constexpr Colour SelectedColour = 0xFEA62A;
	static constexpr Colour ButtonColour = 0x3C3B3C;

private:
	Gui background;
	Text title;
};
}
