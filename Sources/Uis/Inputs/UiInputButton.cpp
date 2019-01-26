#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.2f);
	static const Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const float FONT_SIZE = 1.4f;

	UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_text(std::make_unique<Text>(this, UiBound::Centre, FONT_SIZE, string,
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_primaryColour(primaryColour),
		m_mouseOver(false),
		m_onPressed(Delegate<void(UiInputButton *, bool)>())
	{
		GetRectangle().SetDimensions(SIZE);
	}

	void UiInputButton::UpdateObject()
	{
		// Click updates.
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_onPressed(this, true);
		}

		// Mouse over updates.
		if (m_background->IsSelected() && !m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 0.9f * m_primaryColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}
	}
}
