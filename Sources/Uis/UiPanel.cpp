#include "UiPanel.hpp"

#include "Inputs/UiInputButton.hpp"

namespace acid
{
static const Vector2i RESIZE_SIZE{16, 16};
static const Vector2i PADDING{16, 16};

UiPanel::UiPanel(UiObject *parent, const UiTransform &transform, const Colour &colour, const BitMask<UiManipulate> &manipulate, const BitMask<ScrollBar> &scrollBars) :
	UiObject{parent, transform},
	m_background{this, {UiMargins::All}, Image2d::Create("Guis/White.png"), colour},
	m_content{this, {UiMargins::None, PADDING, -PADDING}},
	m_resizeHandle{this, {RESIZE_SIZE, UiAnchor::RightBottom}, Image2d::Create("Guis/White.png"), UiInputButton::ButtonColour},
	m_manipulate{manipulate},
	m_scrollX{this, ScrollBar::Horizontal, {UiMargins::None, {}, Vector2i{-RESIZE_SIZE.m_x, 0}}},
	m_scrollY{this, ScrollBar::Vertical, {UiMargins::None, {}, Vector2i{0, -RESIZE_SIZE.m_y}}},
	m_scrollBars{scrollBars}
{
	m_resizeHandle.SetCursorHover(CursorStandard::ResizeX);
	m_resizeHandle.SetEnabled(m_manipulate & UiManipulate::Resize);
}

void UiPanel::UpdateObject()
{
	auto contentSize = (m_max - m_min) / GetScreenTransform().GetSize();
	m_scrollX.SetEnabled(m_scrollBars & ScrollBar::Horizontal && contentSize.m_x > 1.05f);
	m_scrollY.SetEnabled(m_scrollBars & ScrollBar::Vertical && contentSize.m_y > 1.05f);

	// TODO: Abstract math into UiTransform.
	m_scrollX.GetTransform().SetAnchor0({0, GetScreenTransform().GetSize().m_y - UiScrollBar::Size});
	m_scrollY.GetTransform().SetAnchor0({GetScreenTransform().GetSize().m_x - UiScrollBar::Size, 0});

	m_scrollX.SetSize({-1.0f / contentSize.m_x, 0.0f});
	m_scrollY.SetSize({0.0f, -1.0f / contentSize.m_y});

	//m_content.GetTransform().SetPosition(0.5f - (Vector2f(m_scrollX.GetProgress(), m_scrollY.GetProgress()) * contentSize));

	m_min = Vector2f::PositiveInfinity;
	m_max = Vector2f::NegativeInfinity;
	//SetScissor(&m_scrollX);
	//SetScissor(&m_scrollY);
	SetScissor(&m_content, true);
}

void UiPanel::SetScissor(UiObject *object, bool checkSize)
{
	auto position = m_background.GetScreenTransform().GetPosition();
	auto size = m_background.GetScreenTransform().GetSize();
	object->SetScissor(Vector4f{position, size});

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
