#include "UiInputText.hpp"

#include "Devices/Keyboard.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputText::UiInputText() {
	m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/Button.png"));
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	this->AddChild(&m_background);

	m_textTitle.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textTitle.SetFontSize(UiInputButton::FontSize);
	m_textTitle.SetJustify(Text::Justify::Right);
	m_textTitle.SetTextColour(UiInputButton::TitleColour);
	this->AddChild(&m_textTitle);

	m_textValue.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textValue.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textValue.SetFontSize(UiInputButton::FontSize);
	m_textValue.SetJustify(Text::Justify::Left);
	m_textValue.SetTextColour(UiInputButton::ValueColour);
	this->AddChild(&m_textValue);

	SetCursorHover(CursorStandard::Hand);
	Keyboard::Get()->OnKey().Add([this](Key key, InputAction action, BitMask<InputMod> mods) {
		if (!m_updating) {
			return;
		}

		if (key == Key::Backspace && action != InputAction::Release) {
			m_inputDelay.Update(true);

			if (m_lastKey != 8 || m_inputDelay.CanInput()) {
				m_value = m_value.substr(0, m_value.length() - 1);
				m_textValue.SetString(m_value);
				m_onValue(m_value);
				m_lastKey = 8;
			}
		} else if (key == Key::Enter && action != InputAction::Release && m_lastKey != 13) {
			m_inputDelay.Update(true);
			SetUpdating(false);
		}
	}, this);
	Keyboard::Get()->OnChar().Add([this](char c) {
		if (!m_updating) {
			return;
		}

		if (m_value.length() < static_cast<uint32_t>(m_maxLength)) {
			m_inputDelay.Update(true);

			if (m_lastKey != c || m_inputDelay.CanInput()) {
				m_value += c;
				m_textValue.SetString(m_value);
				m_onValue(m_value);
				m_lastKey = c;
			}
		} else {
			m_inputDelay.Update(false);
			m_lastKey = 0;
		}
	}, this);
}

void UiInputText::UpdateObject() {
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

void UiInputText::SetUpdating(bool updating) {
	m_updating = updating;
	m_mouseOver = true;
}

void UiInputText::SetValue(const std::string &value) {
	m_value = value;
	m_textValue.SetString(value);
	//m_onValue(m_value);
}
}
