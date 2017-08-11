#include "inputgrabber.h"

namespace flounder
{
	const float inputgrabber::CHANGE_TIME = 0.1f;
	const float inputgrabber::SCALE_NORMAL = 1.6f;
	const float inputgrabber::SCALE_SELECTED = 1.8f;
	colour *const inputgrabber::COLOUR_NORMAL = new colour(0.0f, 0.0f, 0.0f);

	grabberjoystick::grabberjoystick(const int &joystick) :
		igrabber()
	{
		m_joystick = joystick;
	}

	int grabberjoystick::getCurrent(text *object)
	{
		int key = -1;

		if (joysticks::get()->isConnected(m_joystick))
		{
			for (int i = 0; i < joysticks::get()->getCountButtons(m_joystick); i++)
			{
				if (joysticks::get()->getButton(m_joystick, i))
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
		int key = keyboard::get()->getKeyboardChar();

		if (key == 0 || !keyboard::get()->getKey(toupper(key)))
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
			if (mouse::get()->getButton(i))
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

	inputgrabber::inputgrabber(uiobject *parent, const vector2 &position, const std::string &prefix, const int &value, igrabber *grabber, const uialign &align) :
		uiobject(parent, position, vector2(0.0f, 0.0f))
	{
		m_text = new text(this, position, prefix + grabber->getValue(value), SCALE_NORMAL, uis::get()->candara, 0.36f, align);
		m_text->setInScreenCoords(true);
		m_text->setTextColour(colour(1.0f, 1.0f, 1.0f));

		m_background = new gui(this, position, vector2(), new texture("res/guis/buttonText.png"), 1);
		m_background->setInScreenCoords(true);
		m_background->setColourOffset(colour());

		m_grabber = grabber;

		m_prefix = prefix;
		m_value = value;

		m_inputDelay = new inputdelay();
		m_lastKey = 0;

		m_selected = false;
		m_mouseOver = false;

		m_actionChange = nullptr;
	}

	inputgrabber::~inputgrabber()
	{
		delete m_text;
		delete m_background;

		delete m_grabber;

		delete m_inputDelay;
	}

	void inputgrabber::updateObject()
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
				m_text->setScaleDriver(new driverslide(m_text->getScale(), SCALE_NORMAL, CHANGE_TIME));
				uis::get()->getSelector()->cancelWasEvent();
			}
		}

		// Click updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && getAlpha() == 1.0f && uis::get()->getSelector()->wasLeftClick())
		{
			m_text->setScaleDriver(new driverslide(m_text->getScale(), SCALE_SELECTED, CHANGE_TIME));
			m_selected = true;

			uis::get()->getSelector()->cancelWasEvent();
		}
		else if (uis::get()->getSelector()->wasLeftClick() && m_selected)
		{
			m_text->setScaleDriver(new driverslide(m_text->getScale(), SCALE_NORMAL, CHANGE_TIME));
			m_selected = false;
		}

		// Mouse over updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_text->setScaleDriver(new driverslide(m_text->getScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!uis::get()->getSelector()->isSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_text->setScaleDriver(new driverslide(m_text->getScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}

		// Update the background colour.
		colour *primary = uis::get()->getManager()->getPrimaryColour();
		colour::interpolate(*COLOUR_NORMAL, *primary, (m_text->getScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->getColourOffset());

		// Update background size.
		m_background->getDimensions()->set(*m_text->getMeshSize());
		m_background->getDimensions()->m_y = 0.5f * static_cast<float>(m_text->getFontType()->getMetadata()->getMaxSizeY());
		vector2::multiply(*m_text->getDimensions(), *m_background->getDimensions(), m_background->getDimensions());
		m_background->getDimensions()->scale(2.0f * m_text->getScale());
		m_background->getPositionOffsets()->set(*m_text->getPositionOffsets());
		m_background->getPosition()->set(*m_text->getPosition());
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
