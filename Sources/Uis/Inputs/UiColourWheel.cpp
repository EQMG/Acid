#include "UiColourWheel.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
static const Vector2f SIZE = Vector2f(0.22f, 0.22f);

UiColourWheel::UiColourWheel(UiObject *parent, const Colour &value, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/ColourWheel.png")),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f),
	m_value(value)
{
	GetRectangle().SetSize(SIZE);

	OnSelected() += [this](bool selected)
	{
		//m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::AccentColour,
		//	UiInputButton::SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	};
	OnClick() += [this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			CancelEvent(MouseButton::Left);

			Vector2f distance = Mouse::Get()->GetPosition() - (m_background.GetScreenPosition() + (m_background.GetScreenSize() / 2.0f));
			distance /= 0.5f * m_background.GetScreenSize();

			//m_value = Colour(); // TODO: Pick colour.
			m_onValue(m_value);
		}
	};
}

void UiColourWheel::UpdateObject()
{
}

void UiColourWheel::SetValue(const Colour &value)
{
	m_value = value;
	//m_onValue(m_value);
}
}
