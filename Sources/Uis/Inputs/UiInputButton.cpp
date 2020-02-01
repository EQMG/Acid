#include "UiInputButton.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
const Vector2i UiInputButton::Size(175, 28);
const Vector2i UiInputButton::Padding(5, 5);
const float UiInputButton::FontSize(13.0f);
const Time UiInputButton::SlideTime(0.07s);

const Colour UiInputButton::ValueColour(0xFEFCFE);
const Colour UiInputButton::TitleColour(0x9C9A9C);

const Colour UiInputButton::BackgroundColour(0x282729);
const Colour UiInputButton::PrimaryColour(0x121113);
const Colour UiInputButton::SelectedColour(0xFEA62A);
const Colour UiInputButton::ButtonColour(0x3C3B3C);

UiInputButton::UiInputButton() {
	m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_background.SetColourDriver<ConstantDriver>(ButtonColour);
	this->AddChild(&m_background);

	m_title.SetTransform({UiMargins::None, Padding, -Padding});
	m_title.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_title.SetFontSize(FontSize);
	m_title.SetTextColour(ValueColour);
	this->AddChild(&m_title);
	
	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(), selected ? SelectedColour : ButtonColour, SlideTime);
	});
}

void UiInputButton::UpdateObject() {
}
}
