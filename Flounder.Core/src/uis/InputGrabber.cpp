#include "inputgrabber.hpp"

namespace Flounder
{
	const float inputgrabber::CHANGE_TIME = 0.1f;
	const float inputgrabber::SCALE_NORMAL = 1.6f;
	const float inputgrabber::SCALE_SELECTED = 1.8f;
	Colour *const inputgrabber::COLOUR_NORMAL = new Colour(0.0f, 0.0f, 0.0f, 1.0f);

	grabberjoystick::grabberjoystick(const int &joystick) :
		igrabber(),
		m_joystick(joystick)
	{
	}

	int grabberjoystick::getCurrent(text *object)
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
						if (uis::get()->getSelector()->wasLeftClick() && uis::get()->getSelector()->isSelected(*object))
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

	std::string grabberjoystick::getValue(const int &value)
	{
		return std::to_string(value);
	}

	int grabberkeyboard::getCurrent(text *object)
	{
		int key = Keyboard::Get()->GetChar();

		if (key == 0 || !Keyboard::Get()->GetKey(toupper(key)))
		{
			key = -1;
		}

		return key;
	}

	std::string grabberkeyboard::getValue(const int &value)
	{
		return std::string(1, static_cast<char>(value));
	}

	int grabbermouse::getCurrent(text *object)
	{
		int key = -1;

		for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
		{
			if (Mouse::Get()->GetButton(i))
			{
				if (i == 0)
				{
					if (uis::get()->getSelector()->wasLeftClick() && uis::get()->getSelector()->isSelected(*object))
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

	std::string grabbermouse::getValue(const int &value)
	{
		return std::to_string(value);
	}

	inputgrabber::inputgrabber(UiObject *parent, const Vector2 &position, const std::string &prefix, const int &value, igrabber *grabber, const uialign &align) :
		UiObject(parent, position, Vector2(0.0f, 0.0f)),
		m_text(new text(this, position, prefix + grabber->getValue(value), SCALE_NORMAL, uis::get()->m_candara, 0.36f, align)),
		m_background(new gui(this, position, Vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_grabber(grabber),
		m_prefix(prefix),
		m_value(value),
		m_inputDelay(new inputdelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_actionChange(nullptr)
	{
		m_text->SetInScreenCoords(true);
		m_text->setTextColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));

		m_background->SetInScreenCoords(true);
		m_background->setColourOffset(Colour());
	}

	inputgrabber::~inputgrabber()
	{
		delete m_text;
		delete m_background;

		delete m_grabber;

		delete m_inputDelay;
	}

	void inputgrabber::UpdateObject()
	{
		if (m_selected)
		{
			int key = m_grabber->getCurrent(m_text);

			if (key != -1)
			{
				m_value = key;
				m_text->setText(m_prefix + m_grabber->getValue(m_value));

				if (m_actionChange != 0)
				{
					m_actionChange();
				}

				m_selected = false;
				m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
				uis::get()->getSelector()->cancelWasEvent();
			}
		}

		// Click updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && GetAlpha() == 1.0f && uis::get()->getSelector()->wasLeftClick())
		{
			m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_selected = true;

			uis::get()->getSelector()->cancelWasEvent();
		}
		else if (uis::get()->getSelector()->wasLeftClick() && m_selected)
		{
			m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_selected = false;
		}

		// Mouse over updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!uis::get()->getSelector()->isSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}

		// Update the background colour.
		Colour *primary = uis::get()->getManager()->GetPrimaryColour();
		Colour::Interpolate(*COLOUR_NORMAL, *primary, (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->getColourOffset());

		// Update background size.
		m_background->GetDimensions()->set(*m_text->GetMeshSize());
		m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->getFontType()->getMetadata()->getMaxSizeY());
		Vector2::multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		m_background->GetDimensions()->scale(2.0f * m_text->GetScale());
		m_background->GetPositionOffsets()->set(*m_text->GetPositionOffsets());
		m_background->GetPosition()->set(*m_text->GetPosition());
	}

	void inputgrabber::setPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->setText(prefix + m_grabber->getValue(m_value));
	}

	void inputgrabber::setValue(const int &value)
	{
		m_value = value;
		m_text->setText(m_prefix + m_grabber->getValue(value));
	}
}
