#include "UiInputSlider.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	const Time UiInputSlider::CHANGE_TIME = Time::Seconds(0.1f);
	const Time UiInputSlider::SLIDE_TIME = Time::Seconds(0.2f);
	const float UiInputSlider::FONT_SIZE = 1.7f;
	const float UiInputSlider::SCALE_NORMAL = 1.0f;
	const float UiInputSlider::SCALE_SELECTED = 1.1f;

	UiInputSlider::UiInputSlider(UiObject *parent, const std::string &prefix, const float &value,
	                             const float &progressMin, const float &progressMax, const int32_t &roundTo,
	                             const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, false, Vector2(1.0f, 1.0f))),
		m_background(std::make_unique<Gui>(this, rectangle, Texture::Resource("Guis/Button.png"))),
		m_slider(std::make_unique<Gui>(this, rectangle, Texture::Resource("Guis/Button.png"))),
		m_text(std::make_unique<Text>(this, rectangle, FONT_SIZE, prefix, FontType::Resource("Fonts/ProximaNova", "Regular"),
			TEXT_JUSTIFY_CENTRE, rectangle.GetDimensions().m_x, Colour::WHITE)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::IDENTITY, SOUND_TYPE_EFFECT, false, false, 0.9f)),
		m_prefix(prefix),
		m_updating(false),
		m_value(value),
		m_progressMin(progressMin),
		m_progressMax(progressMax),
		m_roundTo(roundTo),
		m_mouseOver(false),
		m_hasChange(false),
		m_timerChange(Timer(SLIDE_TIME)),
		m_onSlide(Delegate<void(UiInputSlider *, float)>())
	{
		m_background->SetColourOffset(primaryColour);
		m_slider->GetRectangle().SetReference(UiBound::CENTRE_LEFT);
		m_slider->SetColourOffset(secondaryColour);
		SetValue(value);
	}

	void UiInputSlider::UpdateObject()
	{
		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT))
		{
			if (!m_updating && !m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_updating = true;
		}
		else if (!Uis::Get()->GetSelector().IsDown(MOUSE_BUTTON_LEFT))
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
			float width = m_background->GetScreenTransform().m_x / 2.0f;
			float positionX = (m_background->GetScreenTransform().m_z + 1.0f) / 2.0f;
			float cursorX = Uis::Get()->GetSelector().GetCursorX() - positionX;
			m_value = cursorX / width;
			m_value = std::clamp(m_value, 0.0f, 1.0f);

			m_hasChange = true;
			CancelEvent(MOUSE_BUTTON_LEFT);
		}

		// Updates the listener.
		if (m_hasChange && m_timerChange.IsPassedTime())
		{
			m_onSlide(this, m_value);

			UpdateText();
			m_hasChange = false;
			m_timerChange.ResetStartTime();
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_slider->SetScaleDriver<DriverSlide>(m_slider->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && !m_updating && m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_slider->SetScaleDriver<DriverSlide>(m_slider->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}

		auto backgroundRectangle = m_background->GetRectangle();
		float aspectRatio = m_background->GetRectangle().IsAspectPosition() ? Display::Get()->GetAspectRatio() : 1.0f;
		m_slider->GetRectangle().SetDimensions(Vector2(backgroundRectangle.GetDimensions().m_x * m_value, 0.05f));
		m_slider->GetRectangle().SetPosition(backgroundRectangle.GetPosition() - Vector2(backgroundRectangle.GetDimensions().m_x / 2.0f / aspectRatio * m_slider->GetScale(), 0.0f));
	}

	void UiInputSlider::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		UpdateText();
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

		if (m_roundTo == 1)
		{
			m_text->SetString(m_prefix + String::To(static_cast<int>(value)));
			return;
		}

		m_text->SetString(m_prefix + String::To(value));
	}
}
