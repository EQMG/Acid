#include "UiInputDropdown.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputDropdown::UiInputDropdown() {
	//slider.SetTransform({{0.5f, 0.0f}, UiAnchor::TopCentre, UiAspect::Position | UiAspect::Scale});
	//slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	//slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	//slider.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	//slider.SetHeight(1.0f);
	//AddChild(&slider);

	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Button.png"));
	background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&background);

	//textTitle.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textTitle.SetFontSize(UiInputButton::FontSize);
	textTitle.SetTextColour(UiInputButton::TitleColour);
	AddChild(&textTitle);

	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(),
			selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime);
	}, this);
}

void UiInputDropdown::UpdateObject() {
	//slider.GetTransform().SetSize({1.0f, 2.0f * static_cast<float>(options.size())});
}

void UiInputDropdown::SetValue(uint32_t value) {
	this->value = value;
	//onValue(value);
}

void UiInputDropdown::SetOptions(const std::vector<std::string> &options) {
	this->options = options;
}
}
