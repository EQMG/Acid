#include <Uis/Inputs/UiInputButton.hpp>
#include "UiPanel.hpp"

#include "Inputs/UiInputButton.hpp"
#include "Uis.hpp"

namespace acid
{
	static const Vector2 SCROLL_BAR = Vector2(0.015f, 0.2f);
	static const float VALUE_SCALE = 1.15f;

	UiPanel::UiPanel(UiObject *parent, const UiBound &rectangle, const bitmask<ScrollBar> &scrollBars) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Panel.png"), UiInputButton::SecondaryColour)),
		m_content(std::make_unique<UiObject>(this, UiBound::Maximum)),
		m_scrollX(std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 1.0f), UiReference::BottomLeft, UiAspect::Position | UiAspect::Dimensions, Vector2(SCROLL_BAR.m_y, SCROLL_BAR.m_x)),
			Texture::Create("Guis/Button_Filled.png"), UiInputButton::PrimaryColour)),
		m_scrollY(std::make_unique<Gui>(this, UiBound(Vector2(1.0f, 0.0f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions, Vector2(SCROLL_BAR.m_x, SCROLL_BAR.m_y)),
			Texture::Create("Guis/Button_Filled.png"), UiInputButton::PrimaryColour)),
		m_scrollBars(scrollBars),
		m_updatingX(false),
		m_updatingY(false),
		m_min(Vector2()),
		m_max(Vector2())
	{
		Mouse::Get()->GetOnScroll() += [this](float xOffset, float yOffset) {
			if (m_background->IsSelected() && GetAlpha() == 1.0f)
			{
				if (!m_updatingX && m_scrollX->IsEnabled())
				{
					float value = ScrollByDelta(-0.06f * xOffset, m_scrollX.get(), m_background.get(), 0);
					m_scrollX->GetRectangle().SetPosition(Vector2(value, 1.0f));
				}

				if (!m_updatingY && m_scrollY->IsEnabled())
				{
					float value = ScrollByDelta(-0.06f * yOffset, m_scrollY.get(), m_background.get(), 1);
					m_scrollY->GetRectangle().SetPosition(Vector2(1.0f, value));
				}
			}
		};
	}

	void UiPanel::UpdateObject()
	{
		Vector2 contentSize = (m_max - m_min) / GetScreenDimension();
		m_scrollX->SetEnabled((m_scrollBars & ScrollBar::Horizontal) && contentSize.m_x > 1.05f);
		m_scrollY->SetEnabled((m_scrollBars & ScrollBar::Vertical) && contentSize.m_y > 1.05f);

		if (m_scrollX->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_updatingX = true;
		}
		else if (!m_scrollX->IsEnabled())
		{
			m_scrollX->GetRectangle().SetPosition(Vector2(0.0f, 1.0f));
		}
		else if (m_updatingX)
		{
			if (!Uis::Get()->IsDown(MouseButton::Left))
			{
				m_updatingX = false;
			}

			float value = ScrollByPosition(Mouse::Get()->GetPositionX(), m_scrollX.get(), m_background.get(), 0);
			m_scrollX->GetRectangle().SetPosition(Vector2(value, 1.0f));
			CancelEvent(MouseButton::Left);
		}

		if (m_scrollY->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_updatingY = true;
		}
		else if (!m_scrollY->IsEnabled())
		{
			m_scrollY->GetRectangle().SetPosition(Vector2(1.0f, 0.0f));
		}
		else if (m_updatingY)
		{
			if (!Uis::Get()->IsDown(MouseButton::Left))
			{
				m_updatingY = false;
			}

			float value = ScrollByPosition(Mouse::Get()->GetPositionY(), m_scrollY.get(), m_background.get(), 1);
			m_scrollY->GetRectangle().SetPosition(Vector2(1.0f, value));
			CancelEvent(MouseButton::Left);
		}

		m_scrollX->GetRectangle().SetDimensions(Vector2(0.5f * (1.0f / contentSize.m_x), SCROLL_BAR.m_x));
		m_scrollY->GetRectangle().SetDimensions(Vector2(SCROLL_BAR.m_x, 0.5f * (1.0f / contentSize.m_y)));
		m_content->GetRectangle().SetPosition(0.5f - (VALUE_SCALE * Vector2(m_scrollX->GetRectangle().GetPosition().m_x, m_scrollY->GetRectangle().GetPosition().m_y) / contentSize));

		m_min = Vector2::PositiveInfinity;
		m_max = Vector2::NegativeInfinity;
		SetScissor(m_scrollX.get());
		SetScissor(m_scrollY.get());
		SetScissor(m_content.get(), true);
	}

	void UiPanel::SetScissor(UiObject *object, const bool &size)
	{
		auto position = m_background->GetScreenPosition();
		auto dimension = m_background->GetScreenDimension();
		object->SetScissor(Vector4(position.m_x, position.m_y, dimension.m_x, dimension.m_y));

		if (object->IsEnabled() && size)
		{
			m_min = Vector2::MinVector(m_min, object->GetScreenPosition());
			m_max = Vector2::MaxVector(m_max, object->GetScreenPosition() + object->GetScreenDimension());
		}

		for (auto &child : object->GetChildren())
		{
			SetScissor(child.get(), size);
		}
	}

	float UiPanel::ScrollByDelta(const float &delta, Gui *puck, UiObject *background, const uint32_t &index)
	{
		float puckLength = puck->GetScreenDimension()[index];
		float barLength = background->GetScreenDimension()[index];
		float maxValue = (barLength - puckLength) / barLength;
		float value = puck->GetRectangle().GetPosition()[index];
		value += delta;
		value = std::clamp(value, 0.0f, maxValue);
		return value;
	}

	float UiPanel::ScrollByPosition(const float &position, Gui *puck, UiObject *background, const uint32_t &index)
	{
		float puckLength = puck->GetScreenDimension()[index];
		float barLength = background->GetScreenDimension()[index];
		float maxValue = (barLength - puckLength) / barLength;
		float positionLength = background->GetScreenPosition()[index];
		float cursorLength = (position - positionLength) - (puckLength / 2.0f);
		return std::clamp(cursorLength / barLength, 0.0f, maxValue);
	}
}
