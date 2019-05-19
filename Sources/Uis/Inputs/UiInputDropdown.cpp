#include "UiInputDropdown.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
UiInputDropdown::UiInputDropdown(UiObject *parent, const std::string &title, const uint32_t &value, const std::vector<std::string> &options, const UiTransform &rectangle) :
	UiObject(parent, rectangle),
	//m_slider(this, UiTransform(Vector2f(0.5f, 0.0f), UiAnchor::TopCentre, UiAspect::Position | UiAspect::Scale), Image2d::Create("Guis/Button_Filled.png"),
	//	UiInputButton::PrimaryColour),
	m_background(this, UiTransform::Maximum, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiTransform(Vector2f(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiAnchor::CentreRight, UiAspect::Position | UiAspect::Size), UiInputButton::FontSize,
		title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, UiInputButton::TitleColour),
	m_value(value),
	m_options(options)
{
	GetTransform().SetSize(UiInputButton::Size);
	//m_slider.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	//m_slider.SetHeight(1.0f);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));

	OnSelected().Add([this](bool selected)
	{
		m_background.SetColourDriver(
			new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
}

void UiInputDropdown::UpdateObject()
{
	//m_slider.GetTransform().SetSize(Vector2f(1.0f, 2.0f * static_cast<float>(m_options.size())));
}

void UiInputDropdown::SetValue(const uint32_t &value)
{
	m_value = value;
	//m_onValue(m_value);
}

void UiInputDropdown::SetOptions(const std::vector<std::string> &options)
{
	m_options = options;
}
}
