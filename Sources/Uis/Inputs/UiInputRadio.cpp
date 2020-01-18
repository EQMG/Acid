#include "UiInputRadio.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputRadio::UiInputRadio(UiObject *parent, const std::string &string, Type type, bool value, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, {{24, 24}, UiAnchor::LeftCentre}, Image2d::Create("Guis/Radio.png"), UiInputButton::PrimaryColour),
	m_fill(&m_background, {{24, 24}, UiAnchor::Centre}, nullptr, UiInputButton::SelectedColour),
	m_text(this, {{140, 24}, UiAnchor::LeftCentre, {29, 0}}, UiInputButton::FontSize, string,
		FontType::Create("Fonts/ProximaNova-Regular.fnt"), Text::Justify::Left, UiInputButton::ValueColour),
	m_value(value),
	m_type(type) {
	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour,
			UiInputButton::SlideTime));
	});
	OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			CancelEvent(MouseButton::Left);

			m_value = !m_value;
			UpdateValue();
			m_onValue(m_value);
		}
	});

	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});

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

	m_fill.SetAlphaDriver(std::make_unique<DriverSlide<float>>(m_fill.GetAlpha(), m_value, UiInputButton::SlideTime));
}
}
