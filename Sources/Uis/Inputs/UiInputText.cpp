#include "UiInputText.hpp"

#include "Devices/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "UiInputButton.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
UiInputText::UiInputText(UiObject* parent, const std::string& title, const std::string& value, const int32_t& maxLength, const UiBound& rectangle)
	: UiObject(parent, rectangle), m_background(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	  m_textTitle(this, UiBound(Vector2(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions), UiInputButton::FontSize, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
				  Colour::White),
	  m_textValue(this, UiBound(Vector2(2.5f * UiInputButton::Padding.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions), UiInputButton::FontSize, value, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f), m_title(title), m_value(value), m_maxLength(maxLength), m_lastKey(0), m_selected(false), m_mouseOver(false)
{
	GetRectangle().SetDimensions(UiInputButton::Size);
	m_background.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));

	Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, BitMask<InputMod> mods) {
		if(!m_selected)
			{
				return;
			}

		if(key == Key::Backspace && action != InputAction::Release)
			{
				m_inputDelay.Update(true);

				if(m_lastKey != 8 || m_inputDelay.CanInput())
					{
						m_value = m_value.substr(0, m_value.length() - 1);
						m_textValue.SetString(m_value);
						m_onType(this, m_value);
						m_lastKey = 8;
					}
			}
		else if(key == Key::Enter && action != InputAction::Release && m_lastKey != 13)
			{
				m_inputDelay.Update(true);
				SetSelected(false);
			}
	};
	Keyboard::Get()->GetOnChar() += [this](char c) {
		if(!m_selected)
			{
				return;
			}

		if(m_value.length() < static_cast<uint32_t>(m_maxLength))
			{
				m_inputDelay.Update(true);

				if(m_lastKey != c || m_inputDelay.CanInput())
					{
						m_value += c;
						m_textValue.SetString(m_value);
						m_onType(this, m_value);
						m_lastKey = c;
					}
			}
		else
			{
				m_inputDelay.Update(false);
				m_lastKey = 0;
			}
	};
}

void UiInputText::UpdateObject()
{
	if(Uis::Get()->WasDown(MouseButton::Left))
		{
			if(m_background.IsSelected())
				{
					SetSelected(true);
					CancelEvent(MouseButton::Left);
				}
			else if(m_selected)
				{
					SetSelected(false);
					CancelEvent(MouseButton::Left);
				}
		}

	if(!m_selected)
		{
			if(m_background.IsSelected() && !m_mouseOver)
				{
					m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
					m_mouseOver = true;
				}
			else if(!m_background.IsSelected() && m_mouseOver)
				{
					m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
					m_mouseOver = false;
				}
		}
}

void UiInputText::SetSelected(const bool& selected)
{
	if(!m_soundClick.IsPlaying())
		{
			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

	m_selected = selected;
	m_mouseOver = true;
}

void UiInputText::SetTitle(const std::string& title)
{
	m_title = title;
	m_textTitle.SetString(m_title);
}

void UiInputText::SetValue(const std::string& value)
{
	m_value = value;
	m_textValue.SetString(value);
}
}
