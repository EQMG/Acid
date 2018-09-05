#include "UiInputSlider.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	const float UiInputSlider::CHANGE_TIME = 0.1f;
	const float UiInputSlider::FONT_SIZE = 1.7f;
	const Vector2 UiInputSlider::DIMENSION = Vector2(0.36f, 0.05f);
	const float UiInputSlider::SCALE_NORMAL = 1.0f;
	const float UiInputSlider::SCALE_SELECTED = 1.1f;

	UiInputSlider::UiInputSlider(UiObject *parent, const Vector3 &position, const std::string &prefix, const int32_t &roundTo, const float &progressMin, const float &progressMax, const float &value) :
		UiObject(parent, UiBound(position, "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(new Text(this, UiBound(position, "Centre", true), FONT_SIZE, prefix, FontType::Resource("Fonts/ProximaNova", "Regular"), JUSTIFY_CENTRE, DIMENSION.m_x)),
		m_background(new Gui(this, UiBound(position, "Centre", true, true, DIMENSION), Texture::Resource("Guis/Button.png"))),
		m_slider(new Gui(this, UiBound(position, "CentreLeft", true, true, DIMENSION), Texture::Resource("Guis/Button.png"))),
		m_prefix(prefix),
		m_roundTo(roundTo),
		m_updating(false),
		m_progressMin(progressMin),
		m_progressMax(progressMax),
		m_value(value),
		m_mouseOver(false),
		m_hasChange(false),
		m_timerChange(Timer(0.2f)),
		m_actionChange(nullptr)
	{
		SetValue(value);
	}

	UiInputSlider::~UiInputSlider()
	{
		delete m_text;
		delete m_background;
		delete m_slider;
	}

	void UiInputSlider::UpdateObject()
	{
		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT))
		{
			m_updating = true;
		}
		else if (!Uis::Get()->GetSelector().IsDown(MOUSE_BUTTON_LEFT))
		{
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
			Uis::Get()->GetSelector().CancelWasEvent();
		}

		// Updates the listener.
		if (m_hasChange && m_timerChange.IsPassedTime())
		{
			if (m_actionChange != nullptr)
			{
				m_actionChange();
			}

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

		float aspectRatio = Display::Get()->GetAspectRatio();
		m_slider->GetRectangle().SetDimensions(Vector2(DIMENSION.m_x * m_value, 0.05f));
		m_slider->GetRectangle().SetPosition(m_background->GetRectangle().GetPosition() - Vector2(DIMENSION.m_x / 2.0f / aspectRatio * m_slider->GetScale(), 0.0f));
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
			m_text->SetString(m_prefix + std::to_string(static_cast<int>(value)));
			return;
		}

		m_text->SetString(m_prefix + std::to_string(value));
	}
}
