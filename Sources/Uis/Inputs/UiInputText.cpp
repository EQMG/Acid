#include "UiInputText.hpp"

#include "Devices/Keyboard.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputText::UiInputText() {
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
	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (!updating)
			return;

		if (key == Key::Backspace && action != InputAction::Release) {
			inputDelay.Update(true);

			if (lastKey != 8 || inputDelay.CanInput()) {
				value = value.substr(0, value.length() - 1);
				textValue.SetString(value);
				onValue(value);
				lastKey = 8;
			}
		} else if (key == Key::Enter && action != InputAction::Release && lastKey != 13) {
			inputDelay.Update(true);
			SetUpdating(false);
		}
	}, this);
	Keyboard::Get()->OnChar().Add([this](char c) {
		if (!updating)
			return;

		if (value.length() < static_cast<uint32_t>(maxLength)) {
			inputDelay.Update(true);

			if (lastKey != c || inputDelay.CanInput()) {
				value += c;
				textValue.SetString(value);
				onValue(value);
				lastKey = c;
			}
		} else {
			inputDelay.Update(false);
			lastKey = 0;
		}
	}, this);
}

void UiInputText::UpdateObject() {
	if (Uis::Get()->WasDown(MouseButton::Left)) {
		if (background.IsSelected()) {
			SetUpdating(true);
			CancelEvent(MouseButton::Left);
		} else if (updating) {
			SetUpdating(false);
			CancelEvent(MouseButton::Left);
		}
	}

	if (!updating) {
		if (background.IsSelected() && !mouseOver) {
			background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
			mouseOver = true;
		} else if (!background.IsSelected() && mouseOver) {
			background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
			mouseOver = false;
		}
	}
}

void UiInputText::SetUpdating(bool updating) {
	this->updating = updating;
	mouseOver = true;
}

void UiInputText::SetValue(const std::string &value) {
	this->value = value;
	textValue.SetString(value);
	//onValue(value);
}
}
