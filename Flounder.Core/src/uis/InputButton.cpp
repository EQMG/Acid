#include "InputButton.hpp"

namespace Flounder
{
	const float InputButton::CHANGE_TIME = 0.1f;
	const float InputButton::SCALE_NORMAL = 1.6f;
	const float InputButton::SCALE_SELECTED = 1.8f;
	Colour *const InputButton::COLOUR_NORMAL = new Colour(0.0f, 0.0f, 0.0f);

	InputButton::InputButton(UiObject *parent, const Vector2 &position, const std::string &string, const UiAlign &align) :
		UiObject(parent, position, Vector2(0.0f, 0.0f)),
		m_text(new Text(this, position, string, SCALE_NORMAL, Uis::get()->m_candara, 0.36f, align)),
		m_background(new Gui(this, position, Vector2(), new Texture("res/guis/buttonText.png"), 1)),
		m_mouseOver(false),
		m_actionLeft(nullptr),
		m_actionRight(nullptr)
	{
		m_text->SetInScreenCoords(true);
		m_text->setTextColour(Colour(1.0f, 1.0f, 1.0f));

		m_background->SetInScreenCoords(true);
		m_background->SetColourOffset(Colour());
	}

	InputButton::~InputButton()
	{
		delete m_text;
		delete m_background;
	}

	void InputButton::UpdateObject()
	{
		// Click updates.
		if (Uis::get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f && Uis::get()->GetSelector()->wasLeftClick())
		{
			if (m_actionLeft != 0)
			{
				m_actionLeft();
			}

			Uis::get()->GetSelector()->CancelWasEvent();
		}
		else if (Uis::get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f && Uis::get()->GetSelector()->wasRightClick())
		{
			if (m_actionRight != 0)
			{
				m_actionRight();
			}

			Uis::get()->GetSelector()->CancelWasEvent();
		}

		// Mouse over updates.
		if (Uis::get()->GetSelector()->IsSelected(*m_text) && !m_mouseOver)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!Uis::get()->GetSelector()->IsSelected(*m_text) && m_mouseOver)
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
}
