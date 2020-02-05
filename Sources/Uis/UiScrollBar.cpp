#include "UiScrollBar.hpp"

#include "Inputs/UiInputButton.hpp"
#include "Drivers/ConstantDriver.hpp"
#include "Drivers/SlideDriver.hpp"
#include "Uis.hpp"

namespace acid {
const uint32_t UiScrollBar::Size = 8;

UiScrollBar::UiScrollBar() {
	//m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/White.png"));
	m_background.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&m_background);

	//m_scroll.SetTransform({UiMargins::RightBottom});
	m_scroll.SetImage(Image2d::Create("Guis/White.png"));
	m_scroll.SetColourDriver<ConstantDriver>(UiInputButton::PrimaryColour);
	AddChild(&m_scroll);

	Mouse::Get()->OnScroll().Add([this](Vector2d wheelDelta) {
		if (GetParent()->IsSelected() && !m_updating && m_scroll.IsEnabled()) {
			Vector2f position;
			position[m_index] = ScrollByDelta(wheelDelta[m_index]);
//			m_scroll.GetTransform().SetPosition(position);
		}
	}, this);
}

void UiScrollBar::UpdateObject() {
	if (m_scroll.IsSelected() && Uis::Get()->WasDown(MouseButton::Left)) {
		m_updating = true;
	} else if (m_updating) {
		if (!Uis::Get()->IsDown(MouseButton::Left)) {
			m_updating = false;
		}

		Vector2d position;
		position[m_index] = Mouse::Get()->GetPosition()[m_index] - GetScreenPosition()[m_index]; // ScrollByPosition(Mouse::Get()->GetPosition()[m_index]);
//		m_scroll.GetTransform().SetPosition(position);
		CancelEvent(MouseButton::Left);
	}

	if (!m_updating) {
		if (m_scroll.IsSelected() && !m_mouseOver) {
			m_scroll.SetColourDriver<SlideDriver>(m_scroll.GetColourDriver()->Get(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
			m_mouseOver = true;
		} else if (!m_scroll.IsSelected() && m_mouseOver) {
			m_scroll.SetColourDriver<SlideDriver>(m_scroll.GetColourDriver()->Get(), UiInputButton::ButtonColour, UiInputButton::SlideTime);
			m_mouseOver = false;
		}
	}
}

float UiScrollBar::GetProgress() {
	// TODO: Mark const
//	return m_scroll.GetTransform().GetPosition()[m_index];
	return 0.0f;
}

void UiScrollBar::SetSize(const Vector2f &size) {
//	m_scroll.GetTransform().SetAnchor0(m_scroll.GetTransform().GetPosition());
//	m_scroll.GetTransform().SetAnchor1(m_scroll.GetTransform().GetPosition() + size);
}

void UiScrollBar::SetType(ScrollBar type) {
	m_index = type == ScrollBar::Vertical;
}

float UiScrollBar::ScrollByDelta(float delta) const {
	float puckLength = m_scroll.GetScreenSize()[m_index];
	float barLength = GetParent()->GetScreenSize()[m_index];
	auto maxValue = (barLength - puckLength) / barLength;
	float value = m_scroll.GetScreenPosition()[m_index];
	value += delta;
	return std::clamp(value, 0.0f, maxValue);
}

float UiScrollBar::ScrollByPosition(float position) const {
	float puckLength = m_scroll.GetScreenSize()[m_index];
	float barLength = GetParent()->GetScreenSize()[m_index];
	auto maxValue = (barLength - puckLength) / barLength;
	float positionLength = GetParent()->GetScreenPosition()[m_index];
	auto cursorLength = (position - positionLength) - (puckLength / 2.0f);
	return std::clamp(cursorLength / barLength, 0.0f, maxValue);
}
}
