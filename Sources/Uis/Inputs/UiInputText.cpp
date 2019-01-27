#include "UiInputText.hpp"

#include "Devices/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const Vector2 PADDING = Vector2(0.01f, 0.07f);
	static const float FONT_SIZE = 1.4f;

	UiInputText::UiInputText(UiObject *parent, const std::string &title, const std::string &value,
	    const int32_t &maxLength, const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_textTitle(std::make_unique<Text>(this, UiBound(Vector2(1.0f - (2.5f * PADDING.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_textValue(std::make_unique<Text>(this, UiBound(Vector2(2.5f * PADDING.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, value, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_title(title),
		m_value(value),
		m_maxLength(maxLength),
		m_inputDelay(InputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_primaryColour(primaryColour),
		m_mouseOver(false),
		m_onType(Delegate<void(UiInputText *, std::string)>())
	{
		GetRectangle().SetDimensions(SIZE);
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
					m_textValue->SetString(m_value);
					m_onType(this, m_value);
					m_lastKey = 8;
				}
			}
			else if (key == Key::Enter && action != InputAction::Release && m_lastKey != 13)
			{
				m_inputDelay.Update(true);

				m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
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
					m_textValue->SetString(m_value);
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
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 1.3f * m_primaryColour, SLIDE_TIME);
			m_selected = true;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();

			CancelEvent(MouseButton::Left);
		}
		else if (Uis::Get()->WasDown(MouseButton::Left) && m_selected)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_selected = false;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		if (m_background->IsSelected() && !m_mouseOver && !m_selected)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 1.3f * m_primaryColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver && !m_selected)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}
	}

	void UiInputText::SetTitle(const std::string &title)
	{
		m_title = title;
		m_textTitle->SetString(m_title);
	}

	void UiInputText::SetValue(const std::string &value)
	{
		m_value = value;
		m_textValue->SetString(value);
	}
}
