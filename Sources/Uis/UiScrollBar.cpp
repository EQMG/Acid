#include "UiScrollBar.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Inputs/UiInputButton.hpp"
#include "Uis.hpp"

namespace acid
{
const uint32_t UiScrollBar::Size{8};

UiScrollBar::UiScrollBar(UiObject *parent, const ScrollBar &type, const UiTransform &transform) :
	UiObject{parent, transform},
	m_background{this, {UiMargins::All}, Image2d::Create("Guis/White.png"), UiInputButton::PrimaryColour},
	m_scroll{this, {UiMargins::RightBottom}, Image2d::Create("Guis/White.png"), UiInputButton::ButtonColour},
	m_index{type == ScrollBar::Vertical}
{
	Mouse::Get()->OnScroll().Add([this](Vector2d wheelDelta)
	{
		if (GetParent()->IsSelected() && !m_updating && m_scroll.IsEnabled())
		{
			Vector2f position;
			position[m_index] = ScrollByDelta(wheelDelta[m_index]);
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

		Vector2d position;
		position[m_index] = Mouse::Get()->GetPosition()[m_index] - GetScreenTransform().GetPosition()[m_index]; //  ScrollByPosition(Mouse::Get()->GetPosition()[m_index]);
		m_scroll.GetTransform().SetPosition(position);
		CancelEvent(MouseButton::Left);
	}

	if (!m_updating)
	{
		if (m_scroll.IsSelected() && !m_mouseOver)
		{
			m_scroll.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_scroll.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime));
			m_mouseOver = true;
		}
		else if (!m_scroll.IsSelected() && m_mouseOver)
		{
			m_scroll.SetColourDriver(std::make_unique<DriverSlide<Colour>>(m_scroll.GetColourOffset(), UiInputButton::ButtonColour, UiInputButton::SlideTime));
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
	m_scroll.GetTransform().SetAnchor0(m_scroll.GetTransform().GetPosition());
	m_scroll.GetTransform().SetAnchor1(m_scroll.GetTransform().GetPosition() + size);
}

float UiScrollBar::ScrollByDelta(const float &delta) const
{
	auto puckLength{m_scroll.GetScreenTransform().m_size[m_index]};
	auto barLength{GetParent()->GetScreenTransform().m_size[m_index]};
	auto maxValue{(barLength - puckLength) / barLength};
	auto value{m_scroll.GetScreenTransform().m_position[m_index]};
	value += delta;
	return std::clamp(value, 0.0f, maxValue);
}

float UiScrollBar::ScrollByPosition(const float &position) const
{
	auto puckLength{m_scroll.GetScreenTransform().m_size[m_index]};
	auto barLength{GetParent()->GetScreenTransform().m_size[m_index]};
	auto maxValue{(barLength - puckLength) / barLength};
	auto positionLength{GetParent()->GetScreenTransform().m_position[m_index]};
	auto cursorLength{(position - positionLength) - (puckLength / 2.0f)};
	return std::clamp(cursorLength / barLength, 0.0f, maxValue);
}
}
