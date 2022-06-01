#include "UiBooleanInput.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiBooleanInput::UiBooleanInput() {
	//slider.SetTransform({UiMargins::All});
	slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	slider.SetColourDriver<ConstantDriver>(UiButtonInput::PrimaryColour);
	AddChild(&slider);

	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Button.png"));
	background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	background.SetColourDriver<ConstantDriver>(UiButtonInput::PrimaryColour);
	AddChild(&background);

	//textTitle.SetTransform({UiMargins::None, UiButtonInput::Padding, -UiButtonInput::Padding});
	textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textTitle.SetFontSize(UiButtonInput::FontSize);
	textTitle.SetJustify(Text::Justify::Right);
	textTitle.SetTextColour(UiButtonInput::TitleColour);
	AddChild(&textTitle);

	//textValue.SetTransform({UiMargins::None, UiButtonInput::Padding, -UiButtonInput::Padding});
	textValue.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textValue.SetFontSize(UiButtonInput::FontSize);
	textValue.SetJustify(Text::Justify::Left);
	textValue.SetTextColour(UiButtonInput::ValueColour);
	AddChild(&textValue);

	SetCursorHover(std::make_unique<Cursor>(CursorStandard::Hand));
	OnSelected().connect([this](bool selected) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), 
			selected ? UiButtonInput::SelectedColour : UiButtonInput::PrimaryColour,
			UiButtonInput::SlideTime);
	});
	OnClick().connect(this, [this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);
			value = !value;
			onValue(value);
			UpdateValue();
		}
	});
	UpdateValue();
}

void UiBooleanInput::UpdateObject() {
}

void UiBooleanInput::SetValue(bool value) {
	this->value = value;
	UpdateValue();
	//onValue(value);
}

void UiBooleanInput::UpdateValue() {
	textValue.SetString(String::To(value));
	slider.SetEnabled(value);
}
}
