#include "UiInputRadio.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputRadio::UiInputRadio() {
	m_background.SetTransform({{24, 24}, UiAnchor::LeftCentre});
	m_background.SetImage(Image2d::Create("Guis/Radio.png"));
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&m_background);

	m_fill.SetTransform({{24, 24}, UiAnchor::Centre});
	m_fill.SetColourDriver<ConstantDriver>(UiInputButton::SelectedColour);
	m_background.AddChild(&m_fill);

	m_title.SetTransform({{140, 24}, UiAnchor::LeftCentre, {29, 0}});
	m_title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_title.SetFontSize(UiInputButton::FontSize);
	m_title.SetTextColour(UiInputButton::ValueColour);
	AddChild(&m_title);
	
	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(),
			selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime);
	});
	OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);

			m_value = !m_value;
			UpdateValue();
			m_onValue(m_value);
		}
	});
	UpdateValue();
}

void UiInputRadio::UpdateObject() {
}

void UiInputRadio::SetValue(bool value) {
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

void UiInputRadio::SetType(const Type &type) {
	m_type = type;
	UpdateValue();
}

void UiInputRadio::UpdateValue() {
	switch (m_type) {
	case Type::Filled:
		m_fill.SetImage(Image2d::Create("Guis/Radio_Filled.png"));
		break;
	case Type::X:
		m_fill.SetImage(Image2d::Create("Guis/Radio_X.png"));
		break;
	case Type::Check:
		m_fill.SetImage(Image2d::Create("Guis/Radio_Check.png"));
		break;
	case Type::Dot:
		m_fill.SetImage(Image2d::Create("Guis/Radio_Dot.png"));
		break;
	}

	m_fill.SetAlphaDriver<SlideDriver>(m_fill.GetAlphaDriver()->Get(), m_value, UiInputButton::SlideTime);
}
}
