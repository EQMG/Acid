#include "UiInputGrabber.hpp"

#include "Inputs/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	const float UiInputGrabber::CHANGE_TIME = 0.1f;
	const float UiInputGrabber::FONT_SIZE = 1.7f;
	const Vector2 UiInputGrabber::DIMENSION = Vector2(0.36f, 0.05f);
	const float UiInputGrabber::SCALE_NORMAL = 1.0f;
	const float UiInputGrabber::SCALE_SELECTED = 1.1f;

	UiGrabberJoystick::UiGrabberJoystick(const JoystickPort &joystick) :
		IUiGrabber(),
		m_joystick(joystick)
	{
	}

	int UiGrabberJoystick::GetCurrent(Text *object)
	{
		int key = -1;

		if (Joysticks::Get()->IsConnected(m_joystick))
		{
			for (uint32_t i = 0; i < Joysticks::Get()->GetCountButtons(m_joystick); i++)
			{
				if (Joysticks::Get()->GetButton(m_joystick, i))
				{
					if (i == 0)
					{
						if (Uis::Get()->GetSelector().WasLeftClick() && Uis::Get()->GetSelector().IsSelected(*object))
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

	std::string UiGrabberJoystick::GetValue(const int &value)
	{
		return std::to_string(value);
	}

	int UiGrabberKeyboard::GetCurrent(Text *object)
	{
		int key = Keyboard::Get()->GetChar();

		if (key == 0 || !Keyboard::Get()->GetKey((Key) toupper(key)))
		{
			key = -1;
		}

		return key;
	}

	std::string UiGrabberKeyboard::GetValue(const int &value)
	{
		return std::string(1, static_cast<char>(value));
	}

	int UiGrabberMouse::GetCurrent(Text *object)
	{
		int key = -1;

		for (int i = 0; i < MOUSE_BUTTON_END_RANGE; i++)
		{
			if (Mouse::Get()->GetButton((MouseButton) i))
			{
				if (i == 0)
				{
					if (Uis::Get()->GetSelector().WasLeftClick() && Uis::Get()->GetSelector().IsSelected(*object))
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

	std::string UiGrabberMouse::GetValue(const int &value)
	{
		return std::to_string(value);
	}

	UiInputGrabber::UiInputGrabber(UiObject *parent, const Vector3 &position, const std::string &prefix, const int &value, IUiGrabber *grabber, const TextJustify &justify) :
		UiObject(parent, UiBound(position, "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(new Text(this, UiBound(position, "Centre", true), FONT_SIZE, prefix + grabber->GetValue(value), FontType::Resource("Fonts/ProximaNova", "Regular"), justify, DIMENSION.m_x)),
		m_background(new Gui(this, UiBound(position, "Centre", true, true, DIMENSION), Texture::Resource("Guis/Button.png"))),
		m_grabber(grabber),
		m_prefix(prefix),
		m_value(value),
		m_inputDelay(new UiInputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_actionChange(nullptr)
	{
	}

	UiInputGrabber::~UiInputGrabber()
	{
		delete m_text;
		delete m_background;

		delete m_grabber;

		delete m_inputDelay;
	}

	void UiInputGrabber::UpdateObject()
	{
		if (m_selected)
		{
			int key = m_grabber->GetCurrent(m_text);

			if (key != -1)
			{
				m_value = key;
				m_text->SetString(m_prefix + m_grabber->GetValue(m_value));

				if (m_actionChange != nullptr)
				{
					m_actionChange();
				}

				m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
				m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
				Uis::Get()->GetSelector().CancelWasEvent();
				m_selected = false;
			}
		}

		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector().WasLeftClick())
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_selected = true;

			Uis::Get()->GetSelector().CancelWasEvent();
		}
		else if (Uis::Get()->GetSelector().WasLeftClick() && m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_selected = false;
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

	void UiInputGrabber::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->SetString(prefix + m_grabber->GetValue(m_value));
	}

	void UiInputGrabber::SetValue(const int &value)
	{
		m_value = value;
		m_text->SetString(m_prefix + m_grabber->GetValue(value));
	}
}
