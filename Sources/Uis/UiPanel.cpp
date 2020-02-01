#include "UiPanel.hpp"

#include "Inputs/UiInputButton.hpp"
#include "Drivers/ConstantDriver.hpp"

namespace acid {
static const Vector2i RESIZE_SIZE(16, 16);
static const Vector2i PADDING(16, 16);

UiPanel::UiPanel() {
	m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/White.png"));
	UiObject::AddChild(&m_background);

	m_content.SetTransform({UiMargins::None, PADDING, -PADDING});
	UiObject::AddChild(&m_content);

	m_resizeHandle.SetTransform({RESIZE_SIZE, UiAnchor::RightBottom});
	m_resizeHandle.SetImage(Image2d::Create("Guis/White.png"));
	m_resizeHandle.SetColourDriver<ConstantDriver>(UiInputButton::ButtonColour);
	m_resizeHandle.SetCursorHover(CursorStandard::ResizeX);
	UiObject::AddChild(&m_resizeHandle);

	m_scrollX.SetTransform({UiMargins::None, {}, {-RESIZE_SIZE.m_x, 0}});
	m_scrollX.SetType(ScrollBar::Horizontal);
	UiObject::AddChild(&m_scrollX);

	m_scrollY.SetTransform({UiMargins::None, {}, {0, -RESIZE_SIZE.m_y}});
	m_scrollY.SetType(ScrollBar::Vertical);
	UiObject::AddChild(&m_scrollY);
}

void UiPanel::UpdateObject() {
	m_resizeHandle.SetEnabled(m_manipulate & UiManipulate::Resize);
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

void UiPanel::AddChild(UiObject *child) {
	m_content.AddChild(child);
}

void UiPanel::SetBackgroundColor(const Colour &colour) {
	m_background.SetColourDriver<ConstantDriver>(colour);
}

void UiPanel::SetScissor(UiObject *object, bool checkSize) {
	auto position = m_background.GetScreenTransform().GetPosition();
	auto size = m_background.GetScreenTransform().GetSize();
	object->SetScissor(Vector4f(position, size));

	if (object->IsEnabled() && checkSize) {
		m_min = m_min.Min(object->GetScreenTransform().GetPosition());
		m_max = m_max.Max(object->GetScreenTransform().GetPosition() + object->GetScreenTransform().GetSize());
	}

	for (auto &child : object->GetChildren()) {
		SetScissor(child, checkSize);
	}
}
}
