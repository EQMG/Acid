#include "UiInputSlider.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "UiInputButton.hpp"
#include "Uis/Uis.hpp"
#include <iomanip>

namespace acid
{
static const Time CHANGE_TIME = Time::Seconds(0.05f);

UiInputSlider::UiInputSlider(UiObject* parent, const std::string& title, const float& value, const float& valueMin, const float& valueMax, const int32_t& roundTo, const UiBound& rectangle)
	: UiObject(parent, rectangle), m_slider(this, UiBound(Vector2(1.0f, 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Scale), Texture::Create("Guis/Button_Filled.png"), UiInputButton::PrimaryColour),
	  m_background(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), UiInputButton::PrimaryColour), m_textTitle(this, UiBound(Vector2(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
																														  UiInputButton::FontSize, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_textValue(this, UiBound(Vector2(2.5f * UiInputButton::Padding.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions), UiInputButton::FontSize, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)), m_title(title), m_updating(false), m_value(value), m_valueMin(valueMin), m_valueMax(valueMax), m_progress(0.0f), m_roundTo(roundTo), m_mouseOver(false),
	  m_hasChange(false), m_timerChange(CHANGE_TIME)
{
	GetRectangle().SetDimensions(UiInputButton::Size);
	m_slider.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
	m_background.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
	SetValue(value);
}

void UiInputSlider::UpdateObject()
{
	if(m_background.IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if(!m_updating && !m_soundClick.IsPlaying())
				{
					m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
					m_soundClick.Play();
				}

			m_updating = true;
			CancelEvent(MouseButton::Left);
		}
	else if(!Uis::Get()->IsDown(MouseButton::Left))
		{
			if(m_updating && !m_soundClick.IsPlaying())
				{
					m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
					m_soundClick.Play();
				}

			m_updating = false;
		}
	else if(m_updating)
		{
			float width = m_background.GetScreenDimensions().m_x;
			float positionX = m_background.GetScreenPosition().m_x;
			float cursorX = Mouse::Get()->GetPositionX() - positionX;
			m_progress = cursorX / width;
			m_progress = std::clamp(m_progress, 0.0f, 1.0f);
			m_value = (m_progress * (m_valueMax - m_valueMin)) + m_valueMin;
			m_onSlide(this, m_value);

			m_hasChange = true;
			CancelEvent(MouseButton::Left);
		}

	if(m_hasChange && m_timerChange.IsPassedTime())
		{
			m_timerChange.ResetStartTime();
			m_hasChange = false;
			UpdateValueText();
		}

	if(m_background.IsSelected() && !m_mouseOver)
		{
			m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
			m_mouseOver = true;
		}
	else if(!m_background.IsSelected() && m_mouseOver && !m_updating)
		{
			m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
			m_mouseOver = false;
		}

	m_slider.GetRectangle().SetDimensions(Vector2(1.0f - m_progress, 1.0f));
}

void UiInputSlider::SetTitle(const std::string& title)
{
	m_title = title;
	m_textTitle.SetString(m_title);
}

void UiInputSlider::SetValue(const float& value)
{
	m_progress = (value - m_valueMin) / (m_valueMax - m_valueMin);
	UpdateValueText();
}

void UiInputSlider::UpdateValueText()
{
	std::stringstream rounded;
	rounded << std::fixed << std::setprecision(m_roundTo) << m_value;
	m_textValue.SetString(rounded.str());
}
}
