#include "UiInputText.hpp"

#include "Devices/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	static const Time CHANGE_TIME = Time::Seconds(0.1f);
	static const float FONT_SIZE = 1.7f;
	static const float SCALE_NORMAL = 1.0f;
	static const float SCALE_SELECTED = 1.1f;

	UiInputText::UiInputText(UiObject *parent, const std::string &prefix, const std::string &value,
	    const int32_t &maxLength, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_text(std::make_unique<Text>(this, UiBound::Centre, FONT_SIZE, prefix + value,
		    FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, rectangle.GetDimensions().m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_prefix(prefix),
		m_value(value),
		m_maxLength(maxLength),
		m_inputDelay(InputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_onType(Delegate<void(UiInputText *, std::string)>())
	{
		Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, bitmask<InputMod> mods) {
			if (!m_selected)
			{
				return;
			}

			if (key == Key::Backspace && action != InputAction::Release)
			{
				m_inputDelay.Update(true);

				if (m_lastKey != 8 || m_inputDelay.CanInput())
				{
					m_value = m_value.substr(0, m_value.length() - 1);
					m_text->SetString(m_prefix + m_value);

					m_onType(this, m_value);

					m_lastKey = 8;
				}
			}
			else if (key == Key::Enter && action != InputAction::Release && m_lastKey != 13)
			{
				m_inputDelay.Update(true);

				m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
				m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
				m_selected = false;

				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}
		};
		Keyboard::Get()->GetOnChar() += [this](char c) {
			if (!m_selected)
			{
				return;
			}

			if (m_value.length() < m_maxLength)
			{
				m_inputDelay.Update(true);

				if (m_lastKey != c || m_inputDelay.CanInput())
				{
					m_value += c;
					m_text->SetString(m_prefix + m_value);

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
		// Click updates.
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_selected = true;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();

			CancelEvent(MouseButton::Left);
		}
		else if (Uis::Get()->WasDown(MouseButton::Left) && m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_selected = false;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		// Mouse over updates.
		if (m_background->IsSelected() && !m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver && !m_selected)
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
