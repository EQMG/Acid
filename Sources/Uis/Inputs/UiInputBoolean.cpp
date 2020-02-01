#include "UiInputBoolean.hpp"

#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"
#include "Uis/Drivers/ConstantDriver.hpp"

namespace acid {
UiInputBoolean::UiInputBoolean() {
	m_slider.SetTransform({UiMargins::All});
	m_slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	m_slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_slider.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	this->AddChild(&m_slider);

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
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour,
			UiInputButton::SlideTime);
	});
	OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);
			m_value = !m_value;
			m_onValue(m_value);
			UpdateValue();
		}
	});
	UpdateValue();
}

void UiInputBoolean::UpdateObject() {
}

void UiInputBoolean::SetValue(bool value) {
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

void UiInputBoolean::UpdateValue() {
	m_textValue.SetString(String::To(m_value));
	m_slider.SetEnabled(m_value);
}
}
