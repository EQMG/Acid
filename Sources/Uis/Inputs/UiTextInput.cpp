#include "UiTextInput.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiTextInput::UiTextInput() {
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
	Windows::Get()->GetWindow(0)->OnKey().connect(this, [this](Key key, InputAction action, bitmask::bitmask<InputMod> mods) {
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
	});
	Windows::Get()->GetWindow(0)->OnChar().connect(this, [this](char c) {
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
	});
}

void UiTextInput::UpdateObject() {
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
			background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), UiButtonInput::SelectedColour, UiButtonInput::SlideTime);
			mouseOver = true;
		} else if (!background.IsSelected() && mouseOver) {
			background.SetColourDriver<SlideDriver>(background.GetColourDriver()->Get(), UiButtonInput::PrimaryColour, UiButtonInput::SlideTime);
			mouseOver = false;
		}
	}
}

void UiTextInput::SetUpdating(bool updating) {
	this->updating = updating;
	mouseOver = true;
}

void UiTextInput::SetValue(const std::string &value) {
	this->value = value;
	textValue.SetString(value);
	//onValue(value);
}
}
