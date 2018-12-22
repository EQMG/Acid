#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	const Time UiInputButton::CHANGE_TIME = Time::Seconds(0.1f);
	const float UiInputButton::FONT_SIZE = 1.7f;
	const float UiInputButton::SCALE_NORMAL = 1.0f;
	const float UiInputButton::SCALE_SELECTED = 1.1f;

	UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, false, Vector2(1.0f, 1.0f))),
		m_background(std::make_unique<Gui>(this, rectangle, Texture::Resource("Guis/Button.png"))),
		m_text(std::make_unique<Text>(this, rectangle, FONT_SIZE, string, FontType::Resource("Fonts/ProximaNova", "Regular"),
			TEXT_JUSTIFY_CENTRE, rectangle.GetDimensions().m_x, Colour::WHITE)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::IDENTITY, SOUND_TYPE_EFFECT, false, false, 0.9f)),
		m_mouseOver(false)
	{
		m_background->SetColourOffset(primaryColour);
	}

	void UiInputButton::UpdateObject()
	{
		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && GetAlpha() == 1.0f &&
		    Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}
}
