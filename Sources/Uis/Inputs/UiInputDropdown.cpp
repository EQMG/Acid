#include "UiInputDropdown.hpp"

#include "Uis/Drivers/ConstantDriver.hpp"
#include "Uis/Drivers/SlideDriver.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputDropdown::UiInputDropdown() {
	//m_slider.SetTransform({{0.5f, 0.0f}, UiAnchor::TopCentre, UiAspect::Position | UiAspect::Scale});
	//m_slider.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	//m_slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	//m_slider.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	//m_slider.SetHeight(1.0f);
	//this->AddChild(&m_slider);

	m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/Button.png"));
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	m_background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	this->AddChild(&m_background);

	m_textTitle.SetTransform({UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding});
	m_textTitle.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textTitle.SetFontSize(UiInputButton::FontSize);
	m_textTitle.SetTextColour(UiInputButton::TitleColour);
	this->AddChild(&m_textTitle);

	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver<SlideDriver>(m_background.GetColourDriver()->Get(),
			selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime);
	});
}

void UiInputDropdown::UpdateObject() {
	//m_slider.GetTransform().SetSize({1.0f, 2.0f * static_cast<float>(m_options.size())});
}

void UiInputDropdown::SetValue(uint32_t value) {
	m_value = value;
	//m_onValue(m_value);
}

void UiInputDropdown::SetOptions(const std::vector<std::string> &options) {
	m_options = options;
}
}
