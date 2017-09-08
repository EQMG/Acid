#include "inputslider.hpp"

namespace Flounder
{
	const float inputslider::CHANGE_TIME = 0.1f;
	const float inputslider::SCALE_NORMAL = 1.6f;
	const float inputslider::SCALE_SELECTED = 1.8f;
	Colour *const inputslider::COLOUR_NORMAL = new Colour(0.0f, 0.0f, 0.0f, 1.0f);

	inputslider::inputslider(UiObject *parent, const Vector2 &position, const std::string &string, const float &progressMin, const float &progressMax, const float &value, const uialign &align) :
		UiObject(parent, position, Vector2(0.0f, 0.0f)),
		m_text(new Text(this, position, string, SCALE_NORMAL, uis::get()->m_candara, 0.36f, align)),
		m_background(new Gui(this, position, Vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_slider(new Gui(this, position, Vector2(), new texture("res/guis/buttonText.png"), 1)),
		m_updating(false),
		m_progressMin(progressMin),
		m_progressMax(progressMax),
		m_mouseOver(false),
		m_hasChange(false),
		m_timerChange(new Timer(0.2f)),
		m_actionChange(nullptr)
	{
		m_text->SetInScreenCoords(true);
		m_text->setTextColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));

		m_background->SetInScreenCoords(true);
		m_background->SetColourOffset(Colour());

		m_slider->SetInScreenCoords(true);
		m_slider->SetColourOffset(Colour());

		setValue(value);
	}

	inputslider::~inputslider()
	{
		delete m_text;
		delete m_background;
		delete m_slider;

		delete m_timerChange;
	}

	void inputslider::UpdateObject()
	{
		// Click updates.
		if (uis::get()->getSelector()->isSelected(*m_text) && GetAlpha() == 1.0f && uis::get()->getSelector()->wasLeftClick())
		{
			if (!m_updating)
			{
				m_updating = true;
			}

			m_hasChange = true;

			float width = 2.0f * m_background->GetMeshSize()->m_x * m_background->GetScreenDimensions()->m_x / static_cast<float>(Display::Get()->GetAspectRatio());
			float positionX = m_background->GetPosition()->m_x;
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
		if (m_hasChange && m_timerChange->IsPassedTime())
		{
			if (m_actionChange != nullptr)
			{
				m_actionChange();
			}

			m_hasChange = false;
			m_timerChange->ResetStartTime();
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
		Colour::Interpolate(*COLOUR_NORMAL, *primary, (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->GetColourOffset());
		m_slider->GetColourOffset()->Set(1.0f - primary->m_r, 1.0f - primary->m_g, 1.0f - primary->m_b, 1.0f);

		// Update background size.
		m_background->GetDimensions()->Set(*m_text->GetMeshSize());
		m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		Vector2::Multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		m_background->GetDimensions()->Scale(2.0f * m_text->GetScale());
		m_background->GetPositionOffsets()->Set(*m_text->GetPositionOffsets());
		m_background->GetPosition()->Set(*m_text->GetPosition());

		// Update slider size. (This is about the worst looking GUI code, but works well.)
		m_slider->GetDimensions()->Set(*m_text->GetMeshSize());
		m_slider->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		Vector2::Multiply(*m_text->GetDimensions(), *m_slider->GetDimensions(), m_slider->GetDimensions());
		m_slider->GetDimensions()->Scale(2.0f * m_text->GetScale());
		m_slider->GetPositionOffsets()->Set(*m_text->GetPositionOffsets());
		m_slider->GetPosition()->Set(*m_text->GetPosition());
		m_slider->GetPositionOffsets()->m_x -= (m_slider->GetDimensions()->m_x / 2.0f);
		m_slider->GetDimensions()->m_x *= m_value;
		m_slider->GetPositionOffsets()->m_x += (m_slider->GetDimensions()->m_x / 2.0f);
	}
}
