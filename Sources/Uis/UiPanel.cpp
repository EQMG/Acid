#include "UiPanel.hpp"

#include "Inputs/UiInputButton.hpp"

namespace acid
{
static const Vector2i RESIZE_SIZE = Vector2i(8, 8);
static const Vector2i PADDING = Vector2i(16, 16);

UiPanel::UiPanel(UiObject *parent, const UiTransform &transform, const Colour &colour, const Resize &resize, const BitMask<ScrollBar> &scrollBars) :
	UiObject(parent, transform),
	m_background(this, UiTransform(transform.GetSize()), Image2d::Create("Guis/White.png"), colour),
	m_content(this, UiTransform(transform.GetSize() - PADDING, UiAnchor::Centre)),
	m_resizeHandle(this, UiTransform(RESIZE_SIZE)),
	m_resize(resize),
	//m_scrollX(this, ScrollBar::Horizontal, UiTransform(UiScrollBar::Size, Vector2i(), UiAnchor::LeftBottom)),
	//m_scrollY(this, ScrollBar::Vertical, UiTransform(UiScrollBar::Size, Vector2i(), UiAnchor::RightTop)),
	m_scrollBars(scrollBars)
{
	switch (m_resize)
	{
	case Resize::Left:
		m_resizeHandle.GetTransform().SetAnchor(UiAnchor::LeftTop);
		break;
	case Resize::Right:
		m_resizeHandle.GetTransform().SetAnchor(UiAnchor::RightTop);
		break;
	default:
		m_resizeHandle.SetEnabled(false);
		break;
	}

	m_resizeHandle.OnSelected().Add([this](bool selected)
	{
		auto standard = m_resize == Resize::Left || m_resize == Resize::Right ? CursorStandard::ResizeX : CursorStandard::ResizeY;
		Mouse::Get()->SetCursor(selected ? standard : CursorStandard::Arrow);
		//GetTransform().SetSize(Mouse::Get()->GetPosition() - GetTransform().GetPosition());
	}, this);
}

void UiPanel::UpdateObject()
{
	m_background.GetTransform().SetSize(GetTransform().GetSize());
	m_content.GetTransform().SetSize(GetTransform().GetSize() - PADDING);

	m_resizeHandle.GetTransform().SetSize(Vector2i(RESIZE_SIZE.m_x, GetTransform().GetSize().m_y));

	/*Vector2f contentSize = (m_max - m_min) / GetScreenTransform().GetSize();
	m_scrollX.SetEnabled((m_scrollBars & ScrollBar::Horizontal) && contentSize.m_x > 1.05f);
	m_scrollY.SetEnabled((m_scrollBars & ScrollBar::Vertical) && contentSize.m_y > 1.05f);

	m_scrollX.SetSize(Vector2f(0.5f * (1.0f / contentSize.m_x), 1.0f));
	m_scrollY.SetSize(Vector2f(1.0f, 0.5f * (1.0f / contentSize.m_y)));*/

	//m_content.GetTransform().SetPosition(0.5f - (Vector2f(m_scrollX.GetProgress(), m_scrollY.GetProgress()) * contentSize));

	m_min = Vector2f::PositiveInfinity;
	m_max = Vector2f::NegativeInfinity;
	//SetScissor(&m_scrollX);
	//SetScissor(&m_scrollY);
	SetScissor(&m_content, true);
}

void UiPanel::SetScissor(UiObject *object, const bool &checkSize)
{
	auto position = m_background.GetScreenTransform().GetPosition();
	auto size = m_background.GetScreenTransform().GetSize();
	object->SetScissor(Vector4i(position.m_x, position.m_y, size.m_x, size.m_y));

	if (object->IsEnabled() && checkSize)
	{
		m_min = m_min.Min(object->GetScreenTransform().GetPosition());
		m_max = m_max.Max(object->GetScreenTransform().GetPosition() + object->GetScreenTransform().GetSize());
	}

	for (auto &child : object->GetChildren())
	{
		SetScissor(child, checkSize);
	}
}
}
