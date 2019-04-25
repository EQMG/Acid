#include "UiPanel.hpp"

#include "Inputs/UiInputButton.hpp"

namespace acid
{
static const float SCROLL_BAR_SIZE = 0.01f;
static const float RESIZE_SIZE = 0.016f;

UiPanel::UiPanel(UiObject *parent, const UiBound &rectangle, const Colour &colour, const Resize &resize, const BitMask<ScrollBar> &scrollBars) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/White.png"), colour),
	m_content(this, UiBound::Maximum),
	m_resizeHandle(this, UiBound()),
	m_resize(resize),
	m_scrollX(this, ScrollBar::Horizontal, UiBound(Vector2f(0.0f, 1.0f), UiReference::BottomLeft, UiAspect::Position | UiAspect::Size, Vector2f(1.0f, SCROLL_BAR_SIZE))),
	m_scrollY(this, ScrollBar::Vertical, UiBound(Vector2f(1.0f, 0.0f), UiReference::TopRight, UiAspect::Position | UiAspect::Size, Vector2f(SCROLL_BAR_SIZE, 1.0f))),
	m_scrollBars(scrollBars)
{
	switch (resize)
	{
	case Resize::Left:
		m_resizeHandle.GetRectangle() = UiBound(Vector2f(RESIZE_SIZE / 2.0f, 0.0f), UiReference::TopLeft, UiAspect::Position | UiAspect::Scale, Vector2f(RESIZE_SIZE, 1.0f));
		break;
	case Resize::Right:
		m_resizeHandle.GetRectangle() = UiBound(Vector2f(1.0f + (RESIZE_SIZE / 2.0f), 0.0f), UiReference::TopRight, UiAspect::Position | UiAspect::Scale, Vector2f(RESIZE_SIZE, 1.0f));
		break;
	default:
		m_resizeHandle.SetEnabled(false);
		break;
	}

	m_resizeHandle.OnSelected().Add([this](bool selected)
	{
		auto standard = m_resize == Resize::Left || m_resize == Resize::Right ? CursorStandard::ResizeX : CursorStandard::ResizeY;
		Mouse::Get()->SetCursor(selected ? standard : CursorStandard::Arrow);
	}, std::ref(*this));
}

void UiPanel::UpdateObject()
{
	Vector2f contentSize = (m_max - m_min) / GetScreenSize();
	m_scrollX.SetEnabled((m_scrollBars & ScrollBar::Horizontal) && contentSize.m_x > 1.05f);
	m_scrollY.SetEnabled((m_scrollBars & ScrollBar::Vertical) && contentSize.m_y > 1.05f);

	m_scrollX.SetSize(Vector2f(0.5f * (1.0f / contentSize.m_x), 1.0f));
	m_scrollY.SetSize(Vector2f(1.0f, 0.5f * (1.0f / contentSize.m_y)));

	//m_content.GetRectangle().SetPosition(0.5f - (Vector2f(m_scrollX.GetProgress(), m_scrollY.GetProgress()) * contentSize));

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
