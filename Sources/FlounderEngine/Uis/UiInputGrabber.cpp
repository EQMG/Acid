﻿#include "UiInputGrabber.hpp"

#include "../Inputs/Mouse.hpp"
#include "../Inputs/Keyboard.hpp"
#include "../Inputs/Joysticks.hpp"
#include "../Scenes/Scenes.hpp"
#include "../Maths/Visual/DriverSlide.hpp"

namespace Flounder
{
	const float UiInputGrabber::CHANGE_TIME = 0.1f;
	const float UiInputGrabber::SCALE_NORMAL = 1.6f;
	const float UiInputGrabber::SCALE_SELECTED = 1.8f;
	const Colour *UiInputGrabber::COLOUR_NORMAL = new Colour("#000000");

	UiGrabberJoystick::UiGrabberJoystick(const unsigned int &joystick) :
		IUiGrabber(),
		m_joystick(joystick)
	{
	}

	int UiGrabberJoystick::GetCurrent(Text *object)
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
						if (Uis::Get()->GetSelector()->WasLeftClick() && Uis::Get()->GetSelector()->IsSelected(*object))
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

		if (key == 0 || !Keyboard::Get()->GetKey(toupper(key)))
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

		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		{
			if (Mouse::Get()->GetButton(i))
			{
				if (i == 0)
				{
					if (Uis::Get()->GetSelector()->WasLeftClick() && Uis::Get()->GetSelector()->IsSelected(*object))
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

	UiInputGrabber::UiInputGrabber(UiObject *parent, const Vector3 &position, const std::string &prefix, const int &value, IUiGrabber *grabber, const FontJustify &justify) :
		UiObject(parent, UiBound(position, "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(nullptr),
		//new Text(this, position, SCALE_NORMAL, Vector3(0.5f, 0.5f, RelativeScreen), prefix + grabber->GetValue(value), Uis::Get()->m_candara->GetRegular(), justify, 0.36f)),
		m_background(nullptr),
		//new Gui(this, position, Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("Resources/Guis/Button.png"), 1)),
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
				m_text->SetText(m_prefix + m_grabber->GetValue(m_value));

				if (m_actionChange != 0)
				{
					m_actionChange();
				}

				m_selected = false;
				m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
				Uis::Get()->GetSelector()->CancelWasEvent();
			}
		}

		// Click updates.
		if (Uis::Get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector()->WasLeftClick())
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_selected = true;

			Uis::Get()->GetSelector()->CancelWasEvent();
		}
		else if (Uis::Get()->GetSelector()->WasLeftClick() && m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_selected = false;
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector()->IsSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector()->IsSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}

		// Update the background colour.
		Colour *primary = Scenes::Get()->GetUiManager()->GetPrimaryColour();
		Colour::Interpolate(*COLOUR_NORMAL, *primary, (m_text->GetScale() - SCALE_NORMAL) /
			(SCALE_SELECTED - SCALE_NORMAL), m_background->GetColourOffset());

		// Update background size.
		//m_background->GetDimensions()->Set(*m_text->GetDimensions());
		//m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		//Vector3::Multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		//m_background->GetDimensions()->Scale(2.0f * m_text->GetScale());
		//m_background->GetPosition()->Set(*m_text->GetPosition());
	}

	void UiInputGrabber::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->SetText(prefix + m_grabber->GetValue(m_value));
	}

	void UiInputGrabber::SetValue(const int &value)
	{
		m_value = value;
		m_text->SetText(m_prefix + m_grabber->GetValue(value));
	}
}
