#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	static const Time CHANGE_TIME = Time::Seconds(0.1f);
	static const float FONT_SIZE = 1.7f;
	static const float SCALE_NORMAL = 1.0f;
	static const float SCALE_SELECTED = 1.1f;

	UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_text(std::make_unique<Text>(this, UiBound::Centre, FONT_SIZE, string,
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, rectangle.GetDimensions().m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_mouseOver(false),
		m_onPressed(Delegate<void(UiInputButton *, bool)>())
	{
	}

	void UiInputButton::UpdateObject()
	{
		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_background) && GetAlpha() == 1.0f &&
		    Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_onPressed(this, true);
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_background) && !m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_background) && m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}
}
