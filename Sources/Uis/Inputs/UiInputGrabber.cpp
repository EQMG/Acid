#include "UiInputGrabber.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputGrabber::UiInputGrabber() {
	m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/Button.png"));
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	m_background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&m_background);

	m_textTitle.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textTitle.SetFontSize(UiInputButton::FontSize);
	m_textTitle.SetJustify(Text::Justify::Right);
	m_textTitle.SetTextColour(UiInputButton::TitleColour);
	AddChild(&m_textTitle);

	m_textValue.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textValue.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textValue.SetFontSize(UiInputButton::FontSize);
	m_textValue.SetJustify(Text::Justify::Left);
	m_textValue.SetTextColour(UiInputButton::ValueColour);
	AddChild(&m_textValue);
	
	SetCursorHover(CursorStandard::Hand);
}

void UiInputGrabber::UpdateObject() {
	if (Uis::Get()->WasDown(MouseButton::Left)) {
		if (m_background.IsSelected()) {
			SetUpdating(true);
			CancelEvent(MouseButton::Left);
		} else if (m_updating) {
			SetUpdating(false);
			CancelEvent(MouseButton::Left);
		}
	}
	if (!m_updating) {
		if (m_background.IsSelected() && !m_mouseOver) {
			m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
			m_mouseOver = true;
		} else if (!m_background.IsSelected() && m_mouseOver) {
			m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
			m_mouseOver = false;
		}
	}
}

void UiInputGrabber::SetUpdating(bool updating) {
	m_updating = updating;
	m_mouseOver = true;
}

void UiInputGrabber::UpdateValue() {
	m_textValue.SetString(GetTextString());
}

UiGrabberJoystick::UiGrabberJoystick() {
	UpdateValue();

	Joysticks::Get()->OnButton().Add([this](uint32_t port, uint32_t button, InputAction action) {
		if (!m_updating || port != m_port) {
			return;
		}

		m_value = button;
		m_onValue(m_port, m_value);
		SetUpdating(false);
		UpdateValue();
	});
}

void UiGrabberJoystick::SetValue(uint32_t value) {
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

UiGrabberKeyboard::UiGrabberKeyboard() {
	UpdateValue();

	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (!m_updating) {
			return;
		}

		m_value = key;
		m_onValue(m_value);
		SetUpdating(false);
		UpdateValue();
	});
}

void UiGrabberKeyboard::SetValue(Key value) {
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

UiGrabberMouse::UiGrabberMouse() {
	UpdateValue();

	Mouse::Get()->OnButton().Add([this](MouseButton button, InputAction action, BitMask<InputMod> mods) {
		if (!m_updating || action != InputAction::Press) {
			return;
		}

		if (button == MouseButton::Left) {
			if (!m_background.IsSelected()) {
				SetUpdating(false);
				return;
			}

			CancelEvent(MouseButton::Left);
		}

		m_value = button;
		m_onValue(m_value);
		SetUpdating(false);
		UpdateValue();
	});
}

void UiGrabberMouse::SetValue(MouseButton value) {
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}
}
