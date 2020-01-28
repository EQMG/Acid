#include "UiInputDropdown.hpp"

#include "Uis/Drivers/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid {
UiInputDropdown::UiInputDropdown(UiObject *parent, const std::string &title, uint32_t value, std::vector<std::string> options, const UiTransform &transform) :
	UiObject(parent, transform),
	//m_slider(this, {{0.5f, 0.0f}, UiAnchor::TopCentre, UiAspect::Position | UiAspect::Scale), Image2d::Create("Guis/Button_Filled.png"),
	//	UiInputButton::PrimaryColour},
	m_background(this, {UiMargins::All}, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, {UiMargins::None, UiInputButton::Padding, -UiInputButton::Padding}, UiInputButton::FontSize, title,
		FontType::Create("Fonts/ProximaNova-Regular.fnt"), Text::Justify::Left, UiInputButton::TitleColour),
	m_value(value),
	m_options(std::move(options)) {
	SetCursorHover(CursorStandard::Hand);
	OnSelected().Add([this](bool selected) {
		m_background.SetColourDriver<DriverSlide>(m_background.GetColourDriver()->Get(),
			selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime);
	});

	//m_slider.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
	//m_slider.SetHeight(1.0f);
	m_background.SetNinePatches({0.125f, 0.125f, 0.875f, 0.875f});
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
