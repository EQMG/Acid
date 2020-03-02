#include "UiDropdownInput.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiDropdownInput::UiDropdownInput() {
	//slider.SetTransform({{0.5f, 0.0f}, UiAnchor::TopCentre, UiAspect::Position | UiAspect::Scale});
	//slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	//slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	//slider.SetColourDriver<ConstantDriver>(UiButtonInput::PrimaryColour);
	//slider.SetHeight(1.0f);
	//AddChild(&slider);

	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Button.png"));
	background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	background.SetColourDriver<ConstantDriver>(UiButtonInput::PrimaryColour);
	AddChild(&background);

	//textTitle.SetTransform({UiMargins::None, UiButtonInput::Padding, -UiButtonInput::Padding});
	textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textTitle.SetFontSize(UiButtonInput::FontSize);
	textTitle.SetTextColour(UiButtonInput::TitleColour);
	AddChild(&textTitle);

	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(),
			selected ? UiButtonInput::SelectedColour : UiButtonInput::PrimaryColour, UiButtonInput::SlideTime);
	}, this);
}

void UiDropdownInput::UpdateObject() {
	//slider.GetTransform().SetSize({1.0f, 2.0f * static_cast<float>(options.size())});
}

void UiDropdownInput::SetValue(uint32_t value) {
	this->value = value;
	//onValue(value);
}

void UiDropdownInput::SetOptions(const std::vector<std::string> &options) {
	this->options = options;
}
}
