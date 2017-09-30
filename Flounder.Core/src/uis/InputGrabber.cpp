#include "InputGrabber.hpp"

#include "../devices/Mouse.hpp"
#include "../devices/Joysticks.hpp"

namespace Flounder
{
	const float InputGrabber::CHANGE_TIME = 0.1f;
	const float InputGrabber::SCALE_NORMAL = 1.6f;
	const float InputGrabber::SCALE_SELECTED = 1.8f;
	Colour *const InputGrabber::COLOUR_NORMAL = new Colour(0.0f, 0.0f, 0.0f);

	GrabberJoystick::GrabberJoystick(const int &joystick) :
		IGrabber(),
		m_joystick(joystick)
	{
	}

	int GrabberJoystick::GetCurrent(Text *object)
	{
		int key = -1;

		if (Joysticks::Get()->IsConnected(m_joystick))
		{
			for (int i = 0; i < Joysticks::Get()->GetCountButtons(m_joystick); i++)
			{
				if (Joysticks::Get()->GetButton(m_joystick, i))
				{
					if (i == 0)
					{
						if (Uis::get()->GetSelector()->wasLeftClick() && Uis::get()->GetSelector()->IsSelected(*object))
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

	std::string GrabberJoystick::GetValue(const int &value)
	{
		return std::to_string(value);
	}

	int GrabberKeyboard::GetCurrent(Text *object)
	{
		int key = Keyboard::Get()->GetChar();

		if (key == 0 || !Keyboard::Get()->GetKey(toupper(key)))
		{
			key = -1;
		}

		return key;
	}

	std::string GrabberKeyboard::GetValue(const int &value)
	{
		return std::string(1, static_cast<char>(value));
	}

	int GrabberMouse::GetCurrent(Text *object)
	{
		int key = -1;

		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		{
			if (Mouse::Get()->GetButton(i))
			{
				if (i == 0)
				{
					if (Uis::get()->GetSelector()->wasLeftClick() && Uis::get()->GetSelector()->IsSelected(*object))
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

	std::string GrabberMouse::GetValue(const int &value)
	{
		return std::to_string(value);
	}

	InputGrabber::InputGrabber(UiObject *parent, const Vector2 &position, const std::string &prefix, const int &value, IGrabber *grabber, const UiAlign &align) :
		UiObject(parent, position, Vector2(0.0f, 0.0f)),
		m_text(new Text(this, position, prefix + grabber->GetValue(value), SCALE_NORMAL, Uis::get()->m_candara, 0.36f, align)),
		m_background(new Gui(this, position, Vector2(), new Texture("res/guis/buttonText.png"), 1)),
		m_grabber(grabber),
		m_prefix(prefix),
		m_value(value),
		m_inputDelay(new InputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_actionChange(nullptr)
	{
		m_text->SetInScreenCoords(true);
		m_text->setTextColour(Colour(1.0f, 1.0f, 1.0f));

		m_background->SetInScreenCoords(true);
		m_background->SetColourOffset(Colour());
	}

	InputGrabber::~InputGrabber()
	{
		delete m_text;
		delete m_background;

		delete m_grabber;

		delete m_inputDelay;
	}

	void InputGrabber::UpdateObject()
	{
		if (m_selected)
		{
			int key = m_grabber->GetCurrent(m_text);

			if (key != -1)
			{
				m_value = key;
				m_text->setText(m_prefix + m_grabber->GetValue(m_value));

				if (m_actionChange != 0)
				{
					m_actionChange();
				}

				m_selected = false;
				m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
				Uis::get()->GetSelector()->CancelWasEvent();
			}
		}

		// Click updates.
		if (Uis::get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f && Uis::get()->GetSelector()->wasLeftClick())
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_selected = true;

			Uis::get()->GetSelector()->CancelWasEvent();
		}
		else if (Uis::get()->GetSelector()->wasLeftClick() && m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_selected = false;
		}

		// Mouse over updates.
		if (Uis::get()->GetSelector()->IsSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!Uis::get()->GetSelector()->IsSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}

		// Update the background colour.
		Colour *primary = Uis::get()->GetManager()->GetPrimaryColour();
		Colour::Interpolate(*COLOUR_NORMAL, *primary, (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->GetColourOffset());

		// Update background size.
		m_background->GetDimensions()->Set(*m_text->GetMeshSize());
		m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		Vector2::Multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		m_background->GetDimensions()->Scale(2.0f * m_text->GetScale());
		m_background->GetPositionOffsets()->Set(*m_text->GetPositionOffsets());
		m_background->GetPosition()->Set(*m_text->GetPosition());
	}

	void InputGrabber::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->setText(prefix + m_grabber->GetValue(m_value));
	}

	void InputGrabber::SetValue(const int &value)
	{
		m_value = value;
		m_text->setText(m_prefix + m_grabber->GetValue(value));
	}
}
