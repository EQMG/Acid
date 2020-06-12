#include "UiRadioInput.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiRadioInput::UiRadioInput() {
	//background.SetTransform({{24, 24}, UiAnchor::LeftCentre});
	background.SetImage(Image2d::Create("Guis/Radio.png"));
	background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	background.SetColourDriver<ConstantDriver>(UiButtonInput::PrimaryColour);
	AddChild(&background);

	//fill.SetTransform({{24, 24}, UiAnchor::Centre});
	fill.SetColourDriver<ConstantDriver>(UiButtonInput::SelectedColour);
	background.AddChild(&fill);

	//title.SetTransform({{140, 24}, UiAnchor::LeftCentre, {29, 0}});
	title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	title.SetFontSize(UiButtonInput::FontSize);
	title.SetTextColour(UiButtonInput::ValueColour);
	AddChild(&title);
	
	SetCursorHover(CursorStandard::Hand);
	OnSelected().connect(this, [this](bool selected) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(),
			selected ? UiButtonInput::SelectedColour : UiButtonInput::PrimaryColour, UiButtonInput::SlideTime);
	});
	OnClick().connect(this, [this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);

			value = !value;
			UpdateValue();
			onValue(value);
		}
	});
	UpdateValue();
}

void UiRadioInput::UpdateObject() {
}

void UiRadioInput::SetValue(bool value) {
	this->value = value;
	UpdateValue();
	//onValue(value);
}

void UiRadioInput::SetType(const Type &type) {
	this->type = type;
	UpdateValue();
}

void UiRadioInput::UpdateValue() {
	switch (type) {
	case Type::Filled:
		fill.SetImage(Image2d::Create("Guis/Radio_Filled.png"));
		break;
	case Type::X:
		fill.SetImage(Image2d::Create("Guis/Radio_X.png"));
		break;
	case Type::Check:
		fill.SetImage(Image2d::Create("Guis/Radio_Check.png"));
		break;
	case Type::Dot:
		fill.SetImage(Image2d::Create("Guis/Radio_Dot.png"));
		break;
	}

	fill.SetAlphaDriver<SlideDriver>(fill.GetAlphaDriver()->Get(), value, UiButtonInput::SlideTime);
}
}
