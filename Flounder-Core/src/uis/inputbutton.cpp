#include "inputbutton.hpp"

namespace flounder
{
	const float inputbutton::CHANGE_TIME = 0.1f;
	const float inputbutton::SCALE_NORMAL = 1.6f;
	const float inputbutton::SCALE_SELECTED = 1.8f;
	colour *const inputbutton::COLOUR_NORMAL = new colour(0.0f, 0.0f, 0.0f);

	inputbutton::inputbutton(uiobject *parent, const vector2 &position, const std::string &string, const uialign &align) :
		uiobject(parent, position, vector2(0.0f, 0.0f)),
		m_text(new text(this, position, string, SCALE_NORMAL, uis::get()->m_candara, 0.36f, align)),
		m_background(new gui(this, position, vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_mouseOver(false),
		m_actionLeft(nullptr),
		m_actionRight(nullptr)
	{
		m_text->setInScreenCoords(true);
		m_text->setTextColour(colour(1.0f, 1.0f, 1.0f));

		m_background->setInScreenCoords(true);
		m_background->setColourOffset(colour());
	}

	inputbutton::~inputbutton()
	{
		delete m_text;
		delete m_background;
	}

	void inputbutton::updateObject()
	{
		// Click updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && getAlpha() == 1.0f && uis::get()->getSelector()->wasLeftClick())
		{
			if (m_actionLeft != 0)
			{
				m_actionLeft();
			}

			uis::get()->getSelector()->cancelWasEvent();
		}
		else if (uis::get()->getSelector()->isSelected(*m_text) && getAlpha() == 1.0f && uis::get()->getSelector()->wasRightClick())
		{
			if (m_actionRight != 0)
			{
				m_actionRight();
			}

			uis::get()->getSelector()->cancelWasEvent();
		}

		// Mouse over updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && !m_mouseOver)
		{
			m_text->setScaleDriver(new driverslide(m_text->getScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!uis::get()->getSelector()->isSelected(*m_text) && m_mouseOver)
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
}
