#include "UiInputSlider.hpp"

#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputSlider::UiInputSlider() {
	//slider.SetTransform( {UiMargins::All});
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

	SetCursorHover(CursorStandard::ResizeX);
	UpdateProgress();
}

void UiInputSlider::UpdateObject() {
	if (background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left)) {
		updating = true;
		CancelEvent(MouseButton::Left);
	} else if (!Uis::Get()->IsDown(MouseButton::Left)) {
		updating = false;
	} else if (updating) {
		auto width = background.GetScreenSize().x;
		auto positionX = background.GetScreenPosition().x;
		auto cursorX = static_cast<float>(Mouse::Get()->GetPosition().x) - positionX;
		progress = cursorX / width;
		progress = std::clamp(progress, 0.0f, 1.0f);
		value = (progress * (valueMax - valueMin)) + valueMin;
		onValue(value);

		CancelEvent(MouseButton::Left);
	}

	UpdateProgress();

	if (background.IsSelected() && !mouseOver) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
		mouseOver = true;
	} else if (!background.IsSelected() && mouseOver && !updating) {
		background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
		mouseOver = false;
	}

//	slider.GetTransform().SetAnchor1({progress - 1.0f, 0.0f});
}

void UiInputSlider::SetValue(float value) {
	this->value = value;
	UpdateProgress();
	//onValue(value);
}

void UiInputSlider::UpdateProgress() {
	progress = (value - valueMin) / (valueMax - valueMin);
	std::stringstream rounded;
	rounded << std::fixed << std::setprecision(roundTo) << value;
	textValue.SetString(rounded.str());
}
}
