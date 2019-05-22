/*#include "UiScrollBar.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Inputs/UiInputButton.hpp"
#include "Uis.hpp"

namespace acid
{
const Vector2i UiScrollBar::Size = Vector2f(16, 16);

const Colour UiScrollBar::BackgroundColour = Colour("#20292b");
const Colour UiScrollBar::PrimaryColour = Colour("#5b7073");
const Colour UiScrollBar::SelectedColour = Colour("#FF6600");

UiScrollBar::UiScrollBar(UiObject *parent, const ScrollBar &type, const UiTransform &transform) :
	UiObject(parent, transform),
	m_background(this, UiTransform::Maximum, Image2d::Create("Guis/White.png"), BackgroundColour),
	m_scroll(this, UiTransform(Vector2f(), UiAnchor::TopLeft, UiAspect::Position | UiAspect::Scale), Image2d::Create("Guis/White.png"), PrimaryColour),
	m_index(type == ScrollBar::Horizontal ? 0 : 1),
	m_updating(false),
	m_mouseOver(false)
{
	Mouse::Get()->OnScroll().Add([this](Vector2f wheelDelta)
	{
		if (GetParent()->IsSelected() && !m_updating && m_scroll.IsEnabled())
		{
			Vector2f position = Vector2f();
			position[m_index] = ScrollByDelta(-0.06f * wheelDelta[m_index]);
			m_scroll.GetTransform().SetPosition(position);
		}
	}, this);
}

void UiScrollBar::UpdateObject()
{
	if (m_scroll.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
	{
		m_updating = true;
	}
	else if (m_updating)
	{
		if (!Uis::Get()->IsDown(MouseButton::Left))
		{
			m_updating = false;
		}

		Vector2f position = Vector2f();
		position[m_index] = ScrollByPosition(Mouse::Get()->GetPosition()[m_index]);
		m_scroll.GetTransform().SetPosition(position);
		CancelEvent(MouseButton::Left);
	}

	if (!m_updating)
	{
		if (m_scroll.IsSelected() && !m_mouseOver)
		{
			m_scroll.SetColourDriver(new DriverSlide<Colour>(m_scroll.GetColourOffset(), SelectedColour, UiInputButton::SlideTime));
			m_mouseOver = true;
		}
		else if (!m_scroll.IsSelected() && m_mouseOver)
		{
			m_scroll.SetColourDriver(new DriverSlide<Colour>(m_scroll.GetColourOffset(), PrimaryColour, UiInputButton::SlideTime));
			m_mouseOver = false;
		}
	}
}

float UiScrollBar::GetProgress()
{
	// TODO: Mark const
	return m_scroll.GetTransform().GetPosition()[m_index];
}

void UiScrollBar::SetSize(const Vector2f &size)
{
	m_scroll.GetTransform().SetScale(size);
}

float UiScrollBar::ScrollByDelta(const float &delta)
{
	float puckLength = m_scroll.GetScreenSize()[m_index];
	float barLength = GetParent()->GetScreenSize()[m_index];
	float maxValue = (barLength - puckLength) / barLength;
	float value = m_scroll.GetTransform().GetPosition()[m_index];
	value += delta;
	return std::clamp(value, 0.0f, maxValue);
}

float UiScrollBar::ScrollByPosition(const float &position)
{
	float puckLength = m_scroll.GetScreenSize()[m_index];
	float barLength = GetParent()->GetScreenSize()[m_index];
	float maxValue = (barLength - puckLength) / barLength;
	float positionLength = GetParent()->GetScreenPosition()[m_index];
	float cursorLength = (position - positionLength) - (puckLength / 2.0f);
	return std::clamp(cursorLength / barLength, 0.0f, maxValue);
}
}*/
