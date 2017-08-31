#include "inputtext.hpp"

namespace Flounder
{
	const float inputtext::CHANGE_TIME = 0.1f;
	const float inputtext::SCALE_NORMAL = 1.6f;
	const float inputtext::SCALE_SELECTED = 1.8f;
	Colour *const inputtext::COLOUR_NORMAL = new Colour(0.0f, 0.0f, 0.0f, 1.0f);

	inputtext::inputtext(UiObject *parent, const Vector2 &position, const std::string &prefix, const std::string &value, const uialign &align) :
		UiObject(parent, position, Vector2(0.0f, 0.0f)),
		m_text(new text(this, position, prefix + value, SCALE_NORMAL, uis::get()->m_candara, 0.36f, align)),
		m_background(new gui(this, position, Vector2(), new texture("res/guis/buttonText.png"), 1)),
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

	inputtext::~inputtext()
	{
		delete m_text;
		delete m_background;

		delete m_inputDelay;
	}

	void inputtext::UpdateObject()
	{
		if (m_selected)
		{
			int key = Keyboard::Get()->GetChar();

			// TODO: Fix inputs that are not GLFW defined.
			if (key != 0 && Keyboard::Get()->GetKey(toupper(key)))
			{
				m_inputDelay->update(true);

				if (m_lastKey != key || m_inputDelay->canInput())
				{
					m_value += static_cast<char>(key);
					m_text->setText(m_prefix + m_value);

					if (m_actionChange != 0)
					{
						m_actionChange();
					}

					m_lastKey = key;
				}
			}
			else if (Keyboard::Get()->GetKey(GLFW_KEY_BACKSPACE))
			{
				m_inputDelay->update(true);

				if (m_lastKey != 8 || m_inputDelay->canInput())
				{
					m_value = m_value.substr(0, m_value.length() - 1);
					m_text->setText(m_prefix + m_value);

					if (m_actionChange != 0)
					{
						m_actionChange();
					}

					m_lastKey = 8;
				}
			}
			else if (Keyboard::Get()->GetKey(GLFW_KEY_ENTER) && m_lastKey != 13)
			{
				m_inputDelay->update(true);

				m_selected = false;
				m_text->SetScaleDriver(new driverslide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			}
			else
			{
				m_inputDelay->update(false);
				m_lastKey = 0;
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
		Colour::Interpolate(*COLOUR_NORMAL, *uis::get()->getManager()->GetPrimaryColour(), (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->getColourOffset());

		// Update background size.
		m_background->GetDimensions()->set(*m_text->GetMeshSize());
		m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->getFontType()->getMetadata()->getMaxSizeY());
		Vector2::multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		m_background->GetDimensions()->scale(2.0f * m_text->GetScale());
		m_background->GetPositionOffsets()->set(*m_text->GetPositionOffsets());
		m_background->GetPosition()->set(*m_text->GetPosition());
	}

	void inputtext::setPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->setText(prefix + m_value);
	}

	void inputtext::setValue(const std::string &value)
	{
		m_value = value;
		m_text->setText(m_prefix + value);
	}
}
