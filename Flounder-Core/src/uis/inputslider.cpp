#include "inputslider.hpp"

namespace Flounder
{
	const float inputslider::CHANGE_TIME = 0.1f;
	const float inputslider::SCALE_NORMAL = 1.6f;
	const float inputslider::SCALE_SELECTED = 1.8f;
	colour *const inputslider::COLOUR_NORMAL = new colour(0.0f, 0.0f, 0.0f, 1.0f);

	inputslider::inputslider(uiobject *parent, const vector2 &position, const std::string &string, const float &progressMin, const float &progressMax, const float &value, const uialign &align) :
		uiobject(parent, position, vector2(0.0f, 0.0f)),
		m_text(new text(this, position, string, SCALE_NORMAL, uis::get()->m_candara, 0.36f, align)),
		m_background(new gui(this, position, vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_slider(new gui(this, position, vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_updating(false),
		m_progressMin(progressMin),
		m_progressMax(progressMax),
		m_mouseOver(false),
		m_hasChange(false),
		m_timerChange(new timer(0.2f)),
		m_actionChange(nullptr)
	{
		m_text->setInScreenCoords(true);
		m_text->setTextColour(colour(1.0f, 1.0f, 1.0f, 1.0f));

		m_background->setInScreenCoords(true);
		m_background->setColourOffset(colour());

		m_slider->setInScreenCoords(true);
		m_slider->setColourOffset(colour());

		setValue(value);
	}

	inputslider::~inputslider()
	{
		delete m_text;
		delete m_background;
		delete m_slider;

		delete m_timerChange;
	}

	void inputslider::updateObject()
	{
		// Click updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && getAlpha() == 1.0f && uis::get()->getSelector()->wasLeftClick())
		{
			if (!m_updating)
			{
				m_updating = true;
			}

			m_hasChange = true;

			float width = 2.0f * m_background->getMeshSize()->m_x * m_background->getScreenDimensions()->m_x / static_cast<float>(display::get()->getAspectRatio());
			float positionX = m_background->getPosition()->m_x;
			float cursorX = uis::get()->getSelector()->getCursorX() - positionX;
			m_value = 2.0f * cursorX / width;
			m_value = (m_value + 1.0f) * 0.5f;

			uis::get()->getSelector()->cancelWasEvent();
		}
		else
		{
			m_updating = false;
		}

		// Updates the listener.
		if (m_hasChange && m_timerChange->isPassedTime())
		{
			if (m_actionChange != nullptr)
			{
				m_actionChange();
			}

			m_hasChange = false;
			m_timerChange->resetStartTime();
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
		m_slider->getColourOffset()->set(1.0f - primary->m_r, 1.0f - primary->m_g, 1.0f - primary->m_b, 1.0f);

		// Update background size.
		m_background->getDimensions()->set(*m_text->getMeshSize());
		m_background->getDimensions()->m_y = 0.5f * static_cast<float>(m_text->getFontType()->getMetadata()->getMaxSizeY());
		vector2::multiply(*m_text->getDimensions(), *m_background->getDimensions(), m_background->getDimensions());
		m_background->getDimensions()->scale(2.0f * m_text->getScale());
		m_background->getPositionOffsets()->set(*m_text->getPositionOffsets());
		m_background->getPosition()->set(*m_text->getPosition());

		// Update slider size. (This is about the worst looking GUI code, but works well.)
		m_slider->getDimensions()->set(*m_text->getMeshSize());
		m_slider->getDimensions()->m_y = 0.5f * static_cast<float>(m_text->getFontType()->getMetadata()->getMaxSizeY());
		vector2::multiply(*m_text->getDimensions(), *m_slider->getDimensions(), m_slider->getDimensions());
		m_slider->getDimensions()->scale(2.0f * m_text->getScale());
		m_slider->getPositionOffsets()->set(*m_text->getPositionOffsets());
		m_slider->getPosition()->set(*m_text->getPosition());
		m_slider->getPositionOffsets()->m_x -= (m_slider->getDimensions()->m_x / 2.0f);
		m_slider->getDimensions()->m_x *= m_value;
		m_slider->getPositionOffsets()->m_x += (m_slider->getDimensions()->m_x / 2.0f);
	}
}
