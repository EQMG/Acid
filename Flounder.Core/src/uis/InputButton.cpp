#include "inputbutton.hpp"

namespace Flounder
{
	const float inputbutton::CHANGE_TIME = 0.1f;
	const float inputbutton::SCALE_NORMAL = 1.6f;
	const float inputbutton::SCALE_SELECTED = 1.8f;
	Colour *const inputbutton::COLOUR_NORMAL = new Colour(0.0f, 0.0f, 0.0f, 1.0f);

	inputbutton::inputbutton(UiObject *parent, const Vector2 &position, const std::string &string, const uialign &align) :
		UiObject(parent, position, Vector2(0.0f, 0.0f)),
		m_text(new text(this, position, string, SCALE_NORMAL, uis::get()->m_candara, 0.36f, align)),
		m_background(new gui(this, position, Vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_mouseOver(false),
		m_actionLeft(nullptr),
		m_actionRight(nullptr)
	{
		m_text->SetInScreenCoords(true);
		m_text->setTextColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));

		m_background->SetInScreenCoords(true);
		m_background->setColourOffset(Colour());
	}

	inputbutton::~inputbutton()
	{
		delete m_text;
		delete m_background;
	}

	void inputbutton::UpdateObject()
	{
		// Click updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && GetAlpha() == 1.0f && uis::get()->getSelector()->wasLeftClick())
		{
			if (m_actionLeft != 0)
			{
				m_actionLeft();
			}

			uis::get()->getSelector()->cancelWasEvent();
		}
		else if (uis::get()->getSelector()->isSelected(*m_text) && GetAlpha() == 1.0f && uis::get()->getSelector()->wasRightClick())
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
			m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!uis::get()->getSelector()->isSelected(*m_text) && m_mouseOver)
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
}
