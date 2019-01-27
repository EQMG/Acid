#include "UiPanel.hpp"

#include "Uis/Uis.hpp"

namespace acid
{
	static const Vector2 SCROLL_BAR = Vector2(0.018f, 0.1f);

	UiPanel::UiPanel(UiObject *parent, const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Panel.png"), primaryColour)),
		m_scrollBarX(std::make_unique<Gui>(this, UiBound(Vector2(0.5f, 1.0f), UiReference::BottomCentre, UiAspect::Position | UiAspect::Dimensions, Vector2(rectangle.GetDimensions().m_x, SCROLL_BAR.m_x)),
			Texture::Create("Guis/Panel.png"), secondaryColour)),
		m_puckX(std::make_unique<Gui>(m_scrollBarX.get(), UiBound(Vector2(), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, Vector2(SCROLL_BAR.m_y, SCROLL_BAR.m_x)),
			Texture::Create("Guis/Panel.png"), 1.4f * secondaryColour)),
		m_scrollBarY(std::make_unique<Gui>(this, UiBound(Vector2(1.0f, 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions, Vector2(SCROLL_BAR.m_x, rectangle.GetDimensions().m_y)),
			Texture::Create("Guis/Panel.png"), secondaryColour)),
		m_puckY(std::make_unique<Gui>(m_scrollBarY.get(), UiBound(Vector2(), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, Vector2(SCROLL_BAR.m_x, SCROLL_BAR.m_y)),
		    Texture::Create("Guis/Panel.png"), 1.4f * secondaryColour)),
		m_content(std::make_unique<UiObject>(this, UiBound::Maximum)),
		m_updatingX(false),
		m_updatingY(false),
		m_min(Vector2()),
		m_max(Vector2())
	{
		m_scrollBarX->SetDepth(10.0f);
		m_puckX->SetDepth(10.1f);
		m_scrollBarY->SetDepth(10.0f);
		m_puckY->SetDepth(10.1f);

		Mouse::Get()->GetOnScroll() += [this](float xOffset, float yOffset) {
			if (m_background->IsSelected() && GetAlpha() == 1.0f)
			{
				if (!m_updatingX && m_scrollBarX->IsEnabled())
				{
					float value = ScrollByDelta(-0.06f * xOffset, m_puckX.get(), m_scrollBarX.get(), 0);
					m_puckX->GetRectangle().SetPosition(Vector2(value, 0.0f));
				}

				if (!m_updatingY && m_scrollBarY->IsEnabled())
				{
					float value = ScrollByDelta(-0.06f * yOffset, m_puckY.get(), m_scrollBarY.get(), 1);
					m_puckY->GetRectangle().SetPosition(Vector2(0.0f, value));
				}
			}
		};
	}

	void UiPanel::UpdateObject()
	{
		Vector2 contentSize = (m_max - m_min) / GetScreenDimension();
		m_scrollBarX->SetEnabled(contentSize.m_x > 1.05f);
		m_scrollBarY->SetEnabled(contentSize.m_y > 1.05f);

		if (m_scrollBarX->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_updatingX = true;
		}
		else if (!m_scrollBarX->IsEnabled())
		{
			m_puckX->GetRectangle().SetPosition(Vector2());
		}
		else if (m_updatingX)
		{
			if (!Uis::Get()->IsDown(MouseButton::Left))
			{
				m_updatingX = false;
			}

			float value = ScrollByPosition(Mouse::Get()->GetPositionX(), m_puckX.get(), m_scrollBarX.get(), 0);
			m_puckX->GetRectangle().SetPosition(Vector2(value, 0.0f));
			CancelEvent(MouseButton::Left);
		}

		if (m_scrollBarY->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_updatingY = true;
		}
		else if (!m_scrollBarY->IsEnabled())
		{
			m_puckY->GetRectangle().SetPosition(Vector2());
		}
		else if (m_updatingY)
		{
			if (!Uis::Get()->IsDown(MouseButton::Left))
			{
				m_updatingY = false;
			}

			float value = ScrollByPosition(Mouse::Get()->GetPositionY(), m_puckY.get(), m_scrollBarY.get(), 1);
			m_puckY->GetRectangle().SetPosition(Vector2(0.0f, value));
			CancelEvent(MouseButton::Left);
		}

		m_content->GetRectangle().SetPosition(0.5f - (Vector2(m_puckX->GetRectangle().GetPosition().m_x, m_puckY->GetRectangle().GetPosition().m_y) / contentSize));

		m_min = Vector2::PositiveInfinity;
		m_max = Vector2::NegativeInfinity;
		SetScissor(m_puckX.get());
		SetScissor(m_puckY.get());
		SetScissor(m_content.get(), true);
	}

	void UiPanel::ResetScrollbars()
	{
		m_puckX->GetRectangle().SetPosition(Vector2());
		m_puckY->GetRectangle().SetPosition(Vector2());
	}

	void UiPanel::SetScissor(UiObject *object, const bool &size)
	{
		auto position = m_background->GetScreenPosition();
		auto dimension = m_background->GetScreenDimension();
		object->SetScissor(Vector4(position.m_x, position.m_y, dimension.m_x, dimension.m_y));

		if (size)
		{
			m_min = Vector2::MinVector(m_min, object->GetScreenPosition());
			m_max = Vector2::MaxVector(m_max, object->GetScreenPosition() + object->GetScreenDimension());
		}

		for (auto &child : object->GetChildren())
		{
			SetScissor(child.get(), size);
		}
	}

	float UiPanel::ScrollByDelta(const float &delta, Gui *puck, Gui *scrollBar, const uint32_t &index)
	{
		float puckLength = puck->GetScreenDimension()[index];
		float barLength = scrollBar->GetScreenDimension()[index];
		float maxValue = (barLength - puckLength) / barLength;
		float value = puck->GetRectangle().GetPosition()[index];
		value += delta;
		value = std::clamp(value, 0.0f, maxValue);
		return value;
	}

	float UiPanel::ScrollByPosition(const float &position, Gui *puck, Gui *scrollBar, const uint32_t &index)
	{
		float puckLength = puck->GetScreenDimension()[index];
		float barLength = scrollBar->GetScreenDimension()[index];
		float maxValue = (barLength - puckLength) / barLength;
		float positionLength = scrollBar->GetScreenPosition()[index];
		float cursorLength = (position - positionLength) - (puckLength / 2.0f);
		return std::clamp(cursorLength / barLength, 0.0f, maxValue);
	}
}
