#include "UiInputGrabber.hpp"

#include "Devices/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	static const Time CHANGE_TIME = Time::Seconds(0.1f);
	static const float FONT_SIZE = 1.7f;
	static const float SCALE_NORMAL = 1.0f;
	static const float SCALE_SELECTED = 1.1f;

	UiInputGrabber::GrabberJoystick::GrabberJoystick(const JoystickPort &joystick) :
		m_joystick(joystick)
	{
	}

	int32_t UiInputGrabber::GrabberJoystick::GetCurrent(Text *object)
	{
		int32_t key = -1;

		if (Joysticks::Get()->IsConnected(m_joystick))
		{
			for (uint32_t i = 0; i < Joysticks::Get()->GetButtonCount(m_joystick); i++)
			{
				if (Joysticks::Get()->GetButton(m_joystick, i))
				{
					if (i == 0)
					{
						if (Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT) && Uis::Get()->GetSelector().IsSelected(*object))
						{
							key = i;
						}
					}
					else
					{
						key = i;
					}
				}
			}
		}

		return key;
	}

	std::string UiInputGrabber::GrabberJoystick::GetValue(const int32_t &value)
	{
		return String::To(value);
	}

	int32_t UiInputGrabber::GrabberKeyboard::GetCurrent(Text *object)
	{
		int32_t key = Keyboard::Get()->GetChar();

		if (key == 0 || !Keyboard::Get()->GetKey((Key) toupper(key)))
		{
			key = -1;
		}

		return key;
	}

	std::string UiInputGrabber::GrabberKeyboard::GetValue(const int32_t &value)
	{
		return std::string(1, static_cast<char>(value));
	}

	int32_t UiInputGrabber::GrabberMouse::GetCurrent(Text *object)
	{
		int32_t key = -1;

		for (uint32_t i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			if (Mouse::Get()->GetButton((MouseButton) i))
			{
				if (i == 0)
				{
					if (Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT) && Uis::Get()->GetSelector().IsSelected(*object))
					{
						key = i;
					}
				}
				else
				{
					key = i;
				}
			}
		}

		return key;
	}

	std::string UiInputGrabber::GrabberMouse::GetValue(const int32_t &value)
	{
		return String::To(value);
	}

	UiInputGrabber::UiInputGrabber(UiObject *parent, const std::string &prefix, const int32_t &value, Grabber *grabber, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, UiBound::Screen),
		m_background(std::make_unique<Gui>(this, rectangle, Texture::Create("Guis/Button.png"))),
		m_text(std::make_unique<Text>(this, rectangle, FONT_SIZE, prefix + grabber->GetValue(value), FontType::Create("Fonts/ProximaNova", "Regular"),
			Text::Justify::Centre, rectangle.GetDimensions().m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_grabber(grabber),
		m_prefix(prefix),
		m_value(value),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_onGrabbed(Delegate<void(UiInputGrabber *, int32_t)>())
	{
		m_background->SetColour(primaryColour);
	}

	void UiInputGrabber::UpdateObject()
	{
		if (m_selected)
		{
			int32_t key = m_grabber->GetCurrent(m_text.get());

			if (key != -1)
			{
				m_value = key;
				m_text->SetString(m_prefix + m_grabber->GetValue(m_value));

				m_onGrabbed(this, m_value);

				m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
				m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
				CancelEvent(MOUSE_BUTTON_LEFT);
				m_selected = false;

				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}
		}

		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_background) && GetAlpha() == 1.0f &&
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
		if (Uis::Get()->GetSelector().IsSelected(*m_background) && !m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_background) && m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}

	void UiInputGrabber::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->SetString(prefix + m_grabber->GetValue(m_value));
	}

	void UiInputGrabber::SetValue(const int32_t &value)
	{
		m_value = value;
		m_text->SetString(m_prefix + m_grabber->GetValue(value));
	}
}
