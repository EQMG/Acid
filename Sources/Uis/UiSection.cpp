#include "UiSection.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
static const Time SLIDE_TIME = Time::Seconds(0.1f);
static const Vector2 SIZE = Vector2(0.3f, 0.03375f);
static const float FONT_SIZE = 1.4f;

UiSection::UiSection(UiObject* parent, const std::string& string, const UiBound& rectangle)
	: UiObject(parent, rectangle), m_icon(this, UiBound::Left, Texture::Create("Guis/Triangle_Down.png")), m_text(this, UiBound::Left, FONT_SIZE, string, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White),
	  m_content(this, UiBound(Vector2(0.0f, 4.0f * SIZE.m_x), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions)), m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f), m_collapsed(false)
{
	GetRectangle().SetDimensions(SIZE);
	m_icon.GetRectangle().SetDimensions(Vector2(GetRectangle().GetDimensions().m_y, GetRectangle().GetDimensions().m_y));
	m_text.GetRectangle().SetPosition(Vector2(4.0f * GetRectangle().GetDimensions().m_y, 0.5f));
}

void UiSection::UpdateObject()
{
	if(m_icon.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if(!m_soundClick.IsPlaying())
				{
					m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
					m_soundClick.Play();
					CancelEvent(MouseButton::Left);
				}

			m_collapsed = !m_collapsed;

			if(m_collapsed)
				{
					m_icon.SetTexture(Texture::Create("Guis/Triangle_Right.png"));
				}
			else
				{
					m_icon.SetTexture(Texture::Create("Guis/Triangle_Down.png"));
				}

			m_onCollapsed(this, m_collapsed);
		}

	m_content.SetEnabled(!m_collapsed);
}
}
