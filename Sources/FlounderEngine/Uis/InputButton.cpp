#include "InputButton.hpp"

namespace Flounder
{
	const float InputButton::CHANGE_TIME = 0.1f;
	const float InputButton::SCALE_NORMAL = 1.6f;
	const float InputButton::SCALE_SELECTED = 1.8f;
	Colour *const InputButton::COLOUR_NORMAL = new Colour("#000000");

	InputButton::InputButton(UiObject *parent, const Vector2 &position, const std::string &string, const Justify &justify) :
		UiObject(parent, UiBound(position, "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(new Text(this, UiBound(position, "Centre"), SCALE_NORMAL, string, Uis::Get()->m_proximaNova->GetRegular(), justify, 0.36f)),
		m_background(new Gui(this, UiBound(position, "Centre", true, true, Vector2(0.36f, 0.05f)), new Texture("Resources/Guis/Button.png"), 1)),
		m_mouseOver(false)
	{
	}

	InputButton::~InputButton()
	{
		delete m_text;
		delete m_background;
	}

	void InputButton::UpdateObject()
	{
		// Mouse over updates.
		/*if (Uis::Get()->GetSelector()->IsSelected(*m_text) && !m_mouseOver)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector()->IsSelected(*m_text) && m_mouseOver)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}*/

		// Update the background colour.
		//	Colour *primary = Uis::Get()->GetManager()->GetPrimaryColour();
		//	Colour::Interpolate(*COLOUR_NORMAL, *primary, (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->GetColourOffset());

		// Update background size.
		//m_background->GetRectangle()->m_dimensions->Set(*m_text->GetRectangle()->m_dimensions);
		//m_background->GetRectangle()->m_dimensions->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		//Vector2::Multiply(*m_text->GetRectangle()->m_dimensions, *m_background->GetRectangle()->m_dimensions, m_background->GetRectangle()->m_dimensions);
		//m_background->GetRectangle()->m_dimensions->Scale(2.0f * m_text->GetScale());
		//m_background->GetRectangle()->m_position->Set(*m_text->GetRectangle()->m_position);
	}
}
