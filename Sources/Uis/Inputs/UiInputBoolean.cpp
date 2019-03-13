#include "UiInputBoolean.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "UiInputButton.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
UiInputBoolean::UiInputBoolean(UiObject* parent, const std::string& title, const bool& value, const UiBound& rectangle)
	: UiObject(parent, rectangle), m_slider(this, UiBound(Vector2(1.0f, 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Scale), Texture::Create("Guis/Button_Filled.png"), UiInputButton::PrimaryColour),
	  m_background(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), UiInputButton::PrimaryColour), m_textTitle(this, UiBound(Vector2(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
																														  UiInputButton::FontSize, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_textValue(this, UiBound(Vector2(2.5f * UiInputButton::Padding.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions), UiInputButton::FontSize, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f), m_title(title), m_value(value), m_mouseOver(false)
{
	GetRectangle().SetDimensions(UiInputButton::Size);
	m_slider.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
	m_background.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
	SetValue(value);
}

void UiInputBoolean::UpdateObject()
{
	if(m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if(!m_soundClick.IsPlaying())
				{
					m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
					m_soundClick.Play();
				}

			CancelEvent(MouseButton::Left);
			m_value = !m_value;
			m_onChange(this, m_value);
			UpdateValueText();
		}

	if(m_background.IsSelected() && !m_mouseOver)
		{
			m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
			m_mouseOver = true;
		}
	else if(!m_background.IsSelected() && m_mouseOver)
		{
			m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
			m_mouseOver = false;
		}

	m_slider.GetRectangle().SetDimensions(Vector2(1.0f - m_value, 1.0f));
}

void UiInputBoolean::SetTitle(const std::string& title)
{
	m_title = title;
	m_textTitle.SetString(m_title);
}

void UiInputBoolean::SetValue(const bool& value)
{
	m_value = value;
	UpdateValueText();
}

void UiInputBoolean::UpdateValueText()
{
	m_textValue.SetString(String::To(m_value));
}
}
