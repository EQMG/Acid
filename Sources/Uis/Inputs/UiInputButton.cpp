#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
const Vector2 UiInputButton::Size = Vector2(0.22f, 0.042f);
const Vector2 UiInputButton::Padding = Vector2(0.01f, 0.07f);
const float UiInputButton::FontSize = 1.3f;
const Time UiInputButton::SlideTime = Time::Seconds(0.12f);

const Colour UiInputButton::PrimaryColour = Colour("#474747");
const Colour UiInputButton::BackgroundColour = Colour("#282828");
const Colour UiInputButton::AccentColour = Colour("#1e88e5");
const Colour UiInputButton::SelectedColour = Colour("#1976d2");

UiInputButton::UiInputButton(UiObject* parent, const std::string& string, const UiBound& rectangle)
	: UiObject(parent, rectangle), m_background(this, UiBound::Maximum, Texture::Create("Guis/Button_Filled.png"), AccentColour), m_text(this, UiBound::Centre, FontSize, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f), m_mouseOver(false)
{
	GetRectangle().SetDimensions(Size);
	m_background.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
}

void UiInputButton::UpdateObject()
{
	if(m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if(!m_soundClick.IsPlaying())
				{
					m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
					m_soundClick.Play();
				}

			CancelEvent(MouseButton::Left);
		}

	if(m_background.IsSelected() && !m_mouseOver)
		{
			m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), SelectedColour, SlideTime);
			m_mouseOver = true;
		}
	else if(!m_background.IsSelected() && m_mouseOver)
		{
			m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), AccentColour, SlideTime);
			m_mouseOver = false;
		}
}
}
