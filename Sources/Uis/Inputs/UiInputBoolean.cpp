#include "UiInputBoolean.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
UiInputBoolean::UiInputBoolean(UiObject *parent, const std::string &title, const bool &value, const UiTransform &rectangle) :
	UiObject(parent, rectangle),
	m_slider(this, UiTransform(Vector2f(1.0f, 0.5f), UiAnchor::CentreRight, UiAspect::Position | UiAspect::Scale), Image2d::Create("Guis/Button_Filled.png"),
		UiInputButton::PrimaryColour),
	m_background(this, UiTransform::Maximum, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiTransform(Vector2f(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiAnchor::CentreRight, UiAspect::Position | UiAspect::Size), UiInputButton::FontSize,
		title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, UiInputButton::TitleColour),
	m_textValue(this, UiTransform(Vector2f(2.5f * UiInputButton::Padding.m_x, 0.5f), UiAnchor::CentreLeft, UiAspect::Position | UiAspect::Size), UiInputButton::FontSize, "",
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, UiInputButton::ValueColour),
	m_value(value)
{
	GetTransform().SetScale(UiInputButton::Size);
	m_slider.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.875f, 0.875f));
	UpdateValue();

	OnSelected().Add([this](bool selected)
	{
		m_background.SetColourDriver(
			new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::PrimaryColour, UiInputButton::SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
	OnClick().Add([this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			CancelEvent(MouseButton::Left);
			m_value = !m_value;
			m_onValue(m_value);
			UpdateValue();
		}
	});
}

void UiInputBoolean::UpdateObject()
{
}

void UiInputBoolean::SetValue(const bool &value)
{
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

void UiInputBoolean::UpdateValue()
{
	m_textValue.SetString(String::To(m_value));
	m_slider.GetTransform().SetScale(Vector2f(1.0f - m_value, 1.0f));
}
}
