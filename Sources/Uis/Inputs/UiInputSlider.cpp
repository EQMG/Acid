#include "UiInputSlider.hpp"

#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputSlider::UiInputSlider() {
	//m_slider.SetTransform( {UiMargins::All});
	m_slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	m_slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_slider.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&m_slider);

	//m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/Button.png"));
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&m_background);

	//m_textTitle.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textTitle.SetFontSize(UiInputButton::FontSize);
	m_textTitle.SetJustify(Text::Justify::Right);
	m_textTitle.SetTextColour(UiInputButton::TitleColour);
	AddChild(&m_textTitle);

	//m_textValue.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textValue.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textValue.SetFontSize(UiInputButton::FontSize);
	m_textValue.SetJustify(Text::Justify::Left);
	m_textValue.SetTextColour(UiInputButton::ValueColour);
	AddChild(&m_textValue);

	SetCursorHover(CursorStandard::ResizeX);
	UpdateProgress();
}

void UiInputSlider::UpdateObject() {
	if (m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left)) {
		m_updating = true;
		CancelEvent(MouseButton::Left);
	} else if (!Uis::Get()->IsDown(MouseButton::Left)) {
		m_updating = false;
	} else if (m_updating) {
		auto width = m_background.GetScreenSize().m_x;
		auto positionX = m_background.GetScreenPosition().m_x;
		auto cursorX = static_cast<float>(Mouse::Get()->GetPosition().m_x) - positionX;
		m_progress = cursorX / width;
		m_progress = std::clamp(m_progress, 0.0f, 1.0f);
		m_value = (m_progress * (m_valueMax - m_valueMin)) + m_valueMin;
		m_onValue(m_value);

		CancelEvent(MouseButton::Left);
	}

	UpdateProgress();

	if (m_background.IsSelected() && !m_mouseOver) {
		m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
		m_mouseOver = true;
	} else if (!m_background.IsSelected() && m_mouseOver && !m_updating) {
		m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
		m_mouseOver = false;
	}

//	m_slider.GetTransform().SetAnchor1({m_progress - 1.0f, 0.0f});
}

void UiInputSlider::SetValue(float value) {
	m_value = value;
	UpdateProgress();
	//m_onValue(m_value);
}

void UiInputSlider::UpdateProgress() {
	m_progress = (m_value - m_valueMin) / (m_valueMax - m_valueMin);
	std::stringstream rounded;
	rounded << std::fixed << std::setprecision(m_roundTo) << m_value;
	m_textValue.SetString(rounded.str());
}
}
