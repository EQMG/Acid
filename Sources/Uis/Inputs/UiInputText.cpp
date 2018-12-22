#include "UiInputText.hpp"

#include "Inputs/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	const Time UiInputText::CHANGE_TIME = Time::Seconds(0.1f);
	const float UiInputText::FONT_SIZE = 1.7f;
	const float UiInputText::SCALE_NORMAL = 1.0f;
	const float UiInputText::SCALE_SELECTED = 1.1f;

	UiInputText::UiInputText(UiObject *parent, const std::string &prefix, const std::string &value,
	                         const int32_t &maxLength, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), UiBound::CENTRE, true, false, Vector2(1.0f, 1.0f))),
		m_background(std::make_unique<Gui>(this, rectangle, Texture::Resource("Guis/Button.png"))),
		m_text(std::make_unique<Text>(this, rectangle, FONT_SIZE, prefix + value, FontType::Resource("Fonts/ProximaNova", "Regular"),
			TEXT_JUSTIFY_CENTRE, rectangle.GetDimensions().m_x, Colour::WHITE)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::IDENTITY, SOUND_TYPE_EFFECT, false, false, 0.9f)),
		m_prefix(prefix),
		m_value(value),
		m_maxLength(maxLength),
		m_inputDelay(InputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_onType(Delegate<void(UiInputText *, std::string)>())
	{
		m_background->SetColourOffset(primaryColour);
	}

	void UiInputText::UpdateObject()
	{
		if (m_selected)
		{
			int32_t key = Keyboard::Get()->GetChar();

			if (m_value.length() < m_maxLength && key != 0 && Keyboard::Get()->GetKey((Key) toupper(key)))
			{
				m_inputDelay.Update(true);

				if (m_lastKey != key || m_inputDelay.CanInput())
				{
					m_value += static_cast<char>(key);
					m_text->SetString(m_prefix + m_value);

					m_onType(this, m_text->GetString());

					m_lastKey = key;
				}
			}
			else if (Keyboard::Get()->GetKey(KEY_BACKSPACE))
			{
				m_inputDelay.Update(true);

				if (m_lastKey != 8 || m_inputDelay.CanInput())
				{
					m_value = m_value.substr(0, m_value.length() - 1);
					m_text->SetString(m_prefix + m_value);

					m_onType(this, m_text->GetString());

					m_lastKey = 8;
				}
			}
			else if (Keyboard::Get()->GetKey(KEY_ENTER) && m_lastKey != 13)
			{
				m_inputDelay.Update(true);

				m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
				m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
				m_selected = false;

				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}
			else
			{
				m_inputDelay.Update(false);
				m_lastKey = 0;
			}
		}

		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT))
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_selected = true;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();

			CancelEvent(MOUSE_BUTTON_LEFT);
		}
		else if (Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT) && m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_selected = false;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}

	void UiInputText::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->SetString(prefix + m_value);
	}

	void UiInputText::SetValue(const std::string &value)
	{
		m_value = value;
		m_text->SetString(m_prefix + value);
	}
}
