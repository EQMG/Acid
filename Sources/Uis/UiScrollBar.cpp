#include "UiScrollBar.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Inputs/UiInputButton.hpp"
#include "Uis.hpp"

namespace acid
{
	UiScrollBar::UiScrollBar(UiObject *parent, const ScrollBar &type, const UiBound &rectangle) :
		UiObject(parent, rectangle),
		m_scroll(std::make_unique<Gui>(this, UiBound(Vector2(), UiReference::TopLeft, UiAspect::Position | UiAspect::Scale),
			Texture::Create("Guis/Button_Filled.png"), UiInputButton::PrimaryColour)),
		m_index(type == ScrollBar::Horizontal ? 0 : 1),
		m_updating(false),
		m_mouseOver(false)
	{
		m_scroll->SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f)); // FIXME

		Mouse::Get()->GetOnScroll() += [this](float xOffset, float yOffset) {
			if (GetParent()->IsSelected() && !m_updating && m_scroll->IsEnabled())
			{
				Vector3 position = Vector2::Zero;
				position[m_index] = ScrollByDelta(-0.06f * (m_index == 0 ? xOffset : yOffset));
				m_scroll->GetRectangle().SetPosition(position);
			}
		};
	}

	void UiScrollBar::UpdateObject()
	{
		if (m_scroll->IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_updating = true;
		}
		else if (m_updating)
		{
			if (!Uis::Get()->IsDown(MouseButton::Left))
			{
				m_updating = false;
			}

			Vector3 position = Vector2::Zero;
			position[m_index] = ScrollByPosition(Mouse::Get()->GetPosition()[m_index]);
			m_scroll->GetRectangle().SetPosition(position);
			CancelEvent(MouseButton::Left);
		}

		if (!m_updating)
		{
			if (m_scroll->IsSelected() && !m_mouseOver)
			{
				m_scroll->SetColourDriver<DriverSlide<Colour>>(m_scroll->GetColourOffset(), UiInputButton::AccentColour, UiInputButton::SlideTime);
				m_mouseOver = true;
			}
			else if (!m_scroll->IsSelected() && m_mouseOver)
			{
				m_scroll->SetColourDriver<DriverSlide<Colour>>(m_scroll->GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
				m_mouseOver = false;
			}
		}
	}

	float UiScrollBar::GetProgress() const
	{
		return m_scroll->GetRectangle().GetPosition()[m_index];
	}

	float UiScrollBar::ScrollByDelta(const float &delta)
	{
		float puckLength = m_scroll->GetScreenDimensions()[m_index];
		float barLength = GetParent()->GetScreenDimensions()[m_index];
		float maxValue = (barLength - puckLength) / barLength;
		float value = m_scroll->GetRectangle().GetPosition()[m_index];
		value += delta;
		return std::clamp(value, 0.0f, maxValue);
	}

	float UiScrollBar::ScrollByPosition(const float &position)
	{
		float puckLength = m_scroll->GetScreenDimensions()[m_index];
		float barLength = GetParent()->GetScreenDimensions()[m_index];
		float maxValue = (barLength - puckLength) / barLength;
		float positionLength = GetParent()->GetScreenPosition()[m_index];
		float cursorLength = (position - positionLength) - (puckLength / 2.0f);
		return std::clamp(cursorLength / barLength, 0.0f, maxValue);
	}
}
