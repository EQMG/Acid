#include "UiColourWheel.hpp"

#include "Uis/Uis.hpp"

namespace acid
{
static const Vector2f SIZE = Vector2f(0.22f, 0.22f);

UiColourWheel::UiColourWheel(UiObject *parent, const Colour &value, const UiTransform &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiTransform::Maximum, Image2d::Create("Guis/ColourWheel.png")),
	m_value(value)
{
	GetTransform().SetScale(SIZE);

	OnSelected().Add([this](bool selected)
	{
		//m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), selected ? UiInputButton::SelectedColour : UiInputButton::AccentColour,
		//	UiInputButton::SlideTime));
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	});
	OnClick().Add([this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			CancelEvent(MouseButton::Left);

			Vector2f distance = Mouse::Get()->GetPosition() - (m_background.GetScreenPosition() + (m_background.GetScreenSize() / 2.0f));
			distance /= 0.5f * m_background.GetScreenSize();

			//m_value = Colour(); // TODO: Pick colour.
			m_onValue(m_value);
		}
	});
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
