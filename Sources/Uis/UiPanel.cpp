#include "UiPanel.hpp"

#include "Inputs/UiInputButton.hpp"

namespace acid
{
static const Vector2f SCROLL_BAR = Vector2f(0.01f, 0.2f);

UiPanel::UiPanel(UiObject *parent, const UiBound &rectangle, const BitMask<ScrollBar> &scrollBars) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/White.png"), UiInputButton::BackgroundColour),
	m_content(this, UiBound::Maximum),
	m_scrollX(this, ScrollBar::Horizontal,
		UiBound(Vector2f(0.0f, 1.0f), UiReference::BottomLeft, UiAspect::Position | UiAspect::Size, Vector2f(SCROLL_BAR.m_y, SCROLL_BAR.m_x))),
	m_scrollY(this, ScrollBar::Vertical, UiBound(Vector2f(1.0f, 0.0f), UiReference::TopRight, UiAspect::Position | UiAspect::Size, Vector2f(SCROLL_BAR.m_x, SCROLL_BAR.m_y))),
	m_scrollBars(scrollBars)
{
}

void UiPanel::UpdateObject()
{
	Vector2f contentSize = (m_max - m_min) / GetScreenSize();
	m_scrollX.SetEnabled((m_scrollBars & ScrollBar::Horizontal) && contentSize.m_x > 1.05f);
	m_scrollY.SetEnabled((m_scrollBars & ScrollBar::Vertical) && contentSize.m_y > 1.05f);

	m_scrollX.GetRectangle().SetSize(Vector2f(0.5f * (1.0f / contentSize.m_x), SCROLL_BAR.m_x));
	m_scrollY.GetRectangle().SetSize(Vector2f(SCROLL_BAR.m_x, 0.5f * (1.0f / contentSize.m_y)));

	//m_content.GetRectangle().SetPosition(0.5f - (Vector2f(m_scrollX->GetProgress(), m_scrollY->GetProgress()) * contentSize));

	m_min = Vector2f::PositiveInfinity;
	m_max = Vector2f::NegativeInfinity;
	SetScissor(&m_scrollX);
	SetScissor(&m_scrollY);
	SetScissor(&m_content, true);
}

void UiPanel::SetScissor(UiObject *object, const bool &checkSize)
{
	auto position = m_background.GetScreenPosition();
	auto size = m_background.GetScreenSize();
	object->SetScissor(Vector4f(position.m_x, position.m_y, size.m_x, size.m_y));

	if (object->IsEnabled() && checkSize)
	{
		m_min = m_min.Min(object->GetScreenPosition());
		m_max = m_max.Max(object->GetScreenPosition() + object->GetScreenSize());
	}

	for (auto &child : object->GetChildren())
	{
		SetScissor(child, checkSize);
	}
}
}
