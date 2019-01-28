#include "UiInputSlider.hpp"

#include <map>
#include <iomanip>
#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Time CHANGE_TIME = Time::Seconds(0.05f);
	static Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const Vector2 PADDING = Vector2(0.01f, 0.07f);
	static const float FONT_SIZE = 1.4f;

	UiInputSlider::UiInputSlider(UiObject *parent, const std::string &title, const float &value, const float &valueMin, const float &valueMax,
		const int32_t &roundTo, const UiBound &rectangle) :
		UiObject(parent, rectangle),
		m_slider(std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Scale),
			Texture::Create("Guis/Button_Filled.png"), UiInputButton::PrimaryColour)),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), UiInputButton::PrimaryColour)),
		m_textTitle(std::make_unique<Text>(this, UiBound(Vector2(1.0f - (2.5f * PADDING.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_textValue(std::make_unique<Text>(this, UiBound(Vector2(2.5f * PADDING.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_title(title),
		m_updating(false),
		m_value(value),
		m_valueMin(valueMin),
		m_valueMax(valueMax),
		m_progress(0.0f),
		m_roundTo(roundTo),
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
		if (m_background->IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_updating && !m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			m_updating = true;
			CancelEvent(MouseButton::Left);
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
			m_progress = cursorX / width;
			m_progress = std::clamp(m_progress, 0.0f, 1.0f);
			m_value = (m_progress * (m_valueMax - m_valueMin)) + m_valueMin;
			m_onSlide(this, m_value);

			m_hasChange = true;
			CancelEvent(MouseButton::Left);
		}

		if (m_hasChange && m_timerChange.IsPassedTime())
		{
			m_timerChange.ResetStartTime();
			m_hasChange = false;
			UpdateValueText();
		}

		if (m_background->IsSelected() && !m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), UiInputButton::SelectedColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver && !m_updating)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), UiInputButton::PrimaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}

		m_slider->GetRectangle().SetDimensions(Vector2(m_progress, 1.0f));
	}

	void UiInputSlider::SetTitle(const std::string &title)
	{
		m_title = title;
		m_textTitle->SetString(m_title);
	}

	void UiInputSlider::SetValue(const float &value)
	{
		m_progress = (value - m_valueMin) / (m_valueMax - m_valueMin);
		UpdateValueText();
	}

	void UiInputSlider::UpdateValueText()
	{
		std::stringstream rounded;
		rounded << std::fixed << std::setprecision(m_roundTo) << m_value;
		m_textValue->SetString(rounded.str());
	}
}
