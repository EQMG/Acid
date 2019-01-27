#include "UiInputSlider.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.2f);
	static const Time CHANGE_TIME = Time::Seconds(0.15f);
	static const Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const Vector2 PADDING = Vector2(0.01f, 0.07f);
	static const float FONT_SIZE = 1.4f;

	UiInputSlider::UiInputSlider(UiObject *parent, const std::string &title, const float &value,
	    const float &progressMin, const float &progressMax, const int32_t &roundTo,
	    const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_slider(std::make_unique<Gui>(m_background.get(), UiBound(Vector2(1.0f - PADDING.m_x, 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Scale),
		    Texture::Create("Guis/Button.png"), secondaryColour)),
		m_textTitle(std::make_unique<Text>(this, UiBound(Vector2(1.0f - (2.0f * PADDING.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_textValue(std::make_unique<Text>(this, UiBound(Vector2(2.0f * PADDING.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_title(title),
		m_updating(false),
		m_value(value),
		m_progressMin(progressMin),
		m_progressMax(progressMax),
		m_roundTo(roundTo),
		m_primaryColour(primaryColour),
		m_mouseOver(false),
		m_hasChange(false),
		m_timerChange(Timer(CHANGE_TIME)),
		m_onSlide(Delegate<void(UiInputSlider *, float)>())
	{
		GetRectangle().SetDimensions(SIZE);
		SetValue(value);
	}

	void UiInputSlider::UpdateObject()
	{
		// Click updates.
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_updating && !m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_updating = true;
		}
		else if (!Uis::Get()->IsDown(MouseButton::Left))
		{
			if (m_updating && !m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_updating = false;
		}
		else if (m_updating)
		{
			float width = m_background->GetScreenDimension().m_x;
			float positionX = m_background->GetScreenPosition().m_x;
			float cursorX = Mouse::Get()->GetPositionX() - positionX;
			m_value = cursorX / width;
			m_value = std::clamp(m_value, 0.0f, 1.0f);
			m_onSlide(this, m_value);

			m_hasChange = true;
			CancelEvent(MouseButton::Left);
		}

		// Updates the listener.
		if (m_hasChange && m_timerChange.IsPassedTime())
		{
			UpdateText();
			m_hasChange = false;
			m_timerChange.ResetStartTime();
		}

		// Mouse over updates.
		if (m_background->IsSelected() && !m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 1.4f * m_primaryColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && !m_updating && m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}

		m_slider->GetRectangle().SetDimensions(Vector2(1.0f - m_value, 1.0f) * (1.0f - (2.0f * PADDING)));
	}

	void UiInputSlider::SetTitle(const std::string &title)
	{
		m_title = title;
		m_textTitle->SetString(m_title);
	}

	void UiInputSlider::SetValue(const float &value)
	{
		m_value = (value - m_progressMin) / (m_progressMax - m_progressMin);
		UpdateText();
	}

	void UiInputSlider::UpdateText()
	{
		float value = (m_value * (m_progressMax - m_progressMin)) + m_progressMin;
		value = Maths::RoundToPlace(value, m_roundTo);
		m_textValue->SetString(String::To(value));
	}
}
