#include "UiButtonInput.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiButtonInput::UiButtonInput() {
	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	background.SetColourDriver<ConstantDriver>(ButtonColour);
	AddChild(&background);

	//title.SetTransform({UiMargins::None, Padding, -Padding});
	title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	title.SetFontSize(FontSize);
	title.SetTextColour(ValueColour);
	AddChild(&title);
	
	SetCursorHover(CursorStandard::Hand);
	OnSelected().connect(this, [this](bool selected) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), selected ? SelectedColour : ButtonColour, SlideTime);
	});
}

void UiButtonInput::UpdateObject() {
}
}
