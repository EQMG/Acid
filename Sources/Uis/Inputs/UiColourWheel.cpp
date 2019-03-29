#include "UiColourWheel.hpp"

#include "Uis/Uis.hpp"

namespace acid
{
static const Vector2 SIZE = Vector2(0.22f, 0.22f);

UiColourWheel::UiColourWheel(UiObject *parent, const Colour &value, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/ColourWheel.png")),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f),
	m_value(value),
	m_mouseOver(false)
{
	GetRectangle().SetDimensions(SIZE);
}

void UiColourWheel::UpdateObject()
{
	if (m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
	{
		if (!m_soundClick.IsPlaying())
		{
			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		Vector2 distance = Mouse::Get()->GetPosition() - (m_background.GetScreenPosition() + (m_background.GetScreenDimensions() / 2.0f));
		distance /= 0.5f * m_background.GetScreenDimensions();

		//m_value = Colour(); // TODO: Pick colour.

		m_onValue(m_value);
	}

	// Mouse over updates.
	if (m_background.IsSelected() && !m_mouseOver)
	{
		//m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), 0.9f * m_primaryColour, SLIDE_TIME));
		m_mouseOver = true;
	}
	else if (!m_background.IsSelected() && m_mouseOver)
	{
		//m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), m_primaryColour, SLIDE_TIME);)
		m_mouseOver = false;
	}
}

void UiColourWheel::SetValue(const Colour &value)
{
	if (m_value != value)
	{
		m_value = value;
		m_onValue(m_value);
	}
}
}
