#include "UiInputGrabber.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputGrabber::UiInputGrabber(UiObject *parent, const std::string &title, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, {UiMargins::All}, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, {UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding}, UiInputButton::FontSize, title,
		FontType::Create("Fonts/ProximaNova"), Text::Justify::Right, UiInputButton::TitleColour),
	m_textValue(this, {UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding}, UiInputButton::FontSize, "",
		FontType::Create("Fonts/ProximaNova"), Text::Justify::Left, UiInputButton::ValueColour) {
	SetCursorHover(CursorStandard::Hand);

	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
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
			m_background.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime));
			m_mouseOver = true;
		} else if (!m_background.IsSelected() && m_mouseOver) {
			m_background.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime));
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

UiGrabberJoystick::UiGrabberJoystick(UiObject *parent, const std::string &title, uint32_t port, uint32_t value, const UiTransform &transform) :
	UiInputGrabber(parent, title, transform),
	m_port(port),
	m_value(value) {
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

UiGrabberKeyboard::UiGrabberKeyboard(UiObject *parent, const std::string &title, Key value, const UiTransform &transform) :
	UiInputGrabber(parent, title, transform),
	m_value(value) {
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

UiGrabberMouse::UiGrabberMouse(UiObject *parent, const std::string &title, MouseButton value, const UiTransform &transform) :
	UiInputGrabber(parent, title, transform),
	m_value(value) {
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
