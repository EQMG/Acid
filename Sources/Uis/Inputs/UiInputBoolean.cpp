#include "UiInputBoolean.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputBoolean::UiInputBoolean() {
	//slider.SetTransform({UiMargins::All});
	slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	slider.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&slider);

	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Button.png"));
	background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&background);

	//textTitle.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textTitle.SetFontSize(UiInputButton::FontSize);
	textTitle.SetJustify(Text::Justify::Right);
	textTitle.SetTextColour(UiInputButton::TitleColour);
	AddChild(&textTitle);

	//textValue.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	textValue.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textValue.SetFontSize(UiInputButton::FontSize);
	textValue.SetJustify(Text::Justify::Left);
	textValue.SetTextColour(UiInputButton::ValueColour);
	AddChild(&textValue);

	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), 
			selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour,
			UiInputButton::SlideTime);
	}, this);
	OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);
			value = !value;
			onValue(value);
			UpdateValue();
		}
	}, this);
	UpdateValue();
}

void UiInputBoolean::UpdateObject() {
}

void UiInputBoolean::SetValue(bool value) {
	this->value = value;
	UpdateValue();
	//onValue(value);
}

void UiInputBoolean::UpdateValue() {
	textValue.SetString(String::To(value));
	slider.SetEnabled(value);
}
}
