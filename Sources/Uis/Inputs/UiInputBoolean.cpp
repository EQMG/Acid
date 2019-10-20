#include "UiInputBoolean.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputBoolean::UiInputBoolean(UiObject *parent, const std::string &title, bool value, const UiTransform &transform) :
	UiObject(parent, transform),
	m_slider(this, {UiMargins::All}, Image2d::Create("Guis/Button_Filled.png"),
		UiInputButton::PrimaryColour),
	m_background(this, {UiMargins::All}, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, {UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding}, UiInputButton::FontSize,
		title, FontType::Create("Fonts/ProximaNova-Regular.ttf"), Text::Justify::Right, UiInputButton::TitleColour),
	m_textValue(this, {UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding}, UiInputButton::FontSize, "",
		FontType::Create("Fonts/ProximaNova-Regular.ttf"), Text::Justify::Left, UiInputButton::ValueColour),
	m_value(value) {
	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour,
			UiInputButton::SlideTime));
	});
	OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);
			m_value = !m_value;
			m_onValue(m_value);
			UpdateValue();
		}
	});

	m_slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
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
