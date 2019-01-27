#include "UiColourWheel.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
//	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Vector2 SIZE = Vector2(0.3f, 0.3f);
//	static const float FONT_SIZE = 1.4f;

	UiColourWheel::UiColourWheel(UiObject *parent, const Colour &colour, const UiBound &rectangle) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/ColourWheel.png"))),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_colour(colour),
		m_mouseOver(false),
		m_onPressed(Delegate<void(UiColourWheel *, Colour)>())
	{
		GetRectangle().SetDimensions(SIZE);
	}

	void UiColourWheel::UpdateObject()
	{
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			Vector2 distance = Mouse::Get()->GetPosition() - (m_background->GetScreenPosition() + (m_background->GetScreenDimension() / 2.0f));
			distance /= 0.5f * m_background->GetScreenDimension();

		//	m_colour = Colour(); // TODO: Pick colour.

			m_onPressed(this, m_colour);
		}

		// Mouse over updates.
		if (m_background->IsSelected() && !m_mouseOver)
		{
		//	m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 0.9f * m_primaryColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver)
		{
		//	m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}
	}

	void UiColourWheel::SetColour(const Colour &colour)
	{
		m_colour = colour;
	}
}
