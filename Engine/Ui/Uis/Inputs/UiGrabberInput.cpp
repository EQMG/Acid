#include "UiGrabberInput.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiGrabberInput::UiGrabberInput() {
	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Button.png"));
	background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
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
}

void UiGrabberInput::UpdateObject() {
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

void UiGrabberInput::SetUpdating(bool updating) {
	this->updating = updating;
	mouseOver = true;
}

void UiGrabberInput::UpdateValue() {
	textValue.SetString(GetTextString());
}

UiGrabberJoystick::UiGrabberJoystick() {
	UpdateValue();

	Joysticks::Get()->GetJoystick(port)->OnButton().connect(this, [this](uint32_t button, InputAction action) {
		if (!updating)
			return;

		value = button;
		onValue(port, value);
		SetUpdating(false);
		UpdateValue();
	});
}

void UiGrabberJoystick::SetValue(uint32_t value) {
	this->value = value;
	UpdateValue();
	//onValue(value);
}

UiGrabberKeyboard::UiGrabberKeyboard() {
	UpdateValue();

	Windows::Get()->GetWindow(0)->OnKey().connect(this, [this](Key key, InputAction action, bitmask::bitmask<InputMod> mods) {
		if (!updating)
			return;

		value = key;
		onValue(value);
		SetUpdating(false);
		UpdateValue();
	});
}

void UiGrabberKeyboard::SetValue(Key value) {
	this->value = value;
	UpdateValue();
	//onValue(value);
}

UiGrabberMouse::UiGrabberMouse() {
	UpdateValue();

	Windows::Get()->GetWindow(0)->OnMouseButton().connect(this, [this](MouseButton button, InputAction action, bitmask::bitmask<InputMod> mods) {
		if (!updating || action != InputAction::Press)
			return;

		if (button == MouseButton::Left) {
			if (!background.IsSelected()) {
				SetUpdating(false);
				return;
			}

			CancelEvent(MouseButton::Left);
		}

		value = button;
		onValue(value);
		SetUpdating(false);
		UpdateValue();
	});
}

void UiGrabberMouse::SetValue(MouseButton value) {
	this->value = value;
	UpdateValue();
	//onValue(value);
}
}
