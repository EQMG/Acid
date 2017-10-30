#include "InputButton.hpp"

#include "../visual/DriverSlide.hpp"
#include "Uis.hpp"

namespace Flounder
{
	const float InputButton::CHANGE_TIME = 0.1f;
	const float InputButton::SCALE_NORMAL = 1.6f;
	const float InputButton::SCALE_SELECTED = 1.8f;
	Colour *const InputButton::COLOUR_NORMAL = new Colour("#000000");

	InputButton::InputButton(UiObject *parent, const Vector3 &position, const std::string &string, const Justify &justify) :
		UiObject(parent, position, Vector3(0.0f, 0.0f, RelativeScreen), Vector2(0.5f, 0.5f)),
		m_text(new Text(this, position, SCALE_NORMAL, Vector3(0.5f, 0.5f, RelativeScreen), string, Uis::Get()->m_candara->GetRegular(), justify, 0.36f)),
		m_background(new Gui(this, position, Vector3(0.36f, 0.05f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("res/guis/buttonText.png"), 1)),
		m_mouseOver(false),
		m_actionLeft(nullptr),
		m_actionRight(nullptr)
	{
	}

	InputButton::~InputButton()
	{
		delete m_text;
		delete m_background;
	}

	void InputButton::UpdateObject()
	{
		// Click updates.
		if (Uis::Get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f && Uis::Get()->GetSelector()->wasLeftClick())
		{
			if (m_actionLeft != 0)
			{
				m_actionLeft();
			}

			Uis::Get()->GetSelector()->CancelWasEvent();
		}
		else if (Uis::Get()->GetSelector()->IsSelected(*m_text) && GetAlpha() == 1.0f && Uis::Get()->GetSelector()->wasRightClick())
		{
			if (m_actionRight != 0)
			{
				m_actionRight();
			}

			Uis::Get()->GetSelector()->CancelWasEvent();
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector()->IsSelected(*m_text) && !m_mouseOver)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME));
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector()->IsSelected(*m_text) && m_mouseOver)
		{
			m_text->SetScaleDriver(new DriverSlide(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME));
			m_mouseOver = false;
		}

		// Update the background colour.
		Colour *primary = Uis::Get()->GetManager()->GetPrimaryColour();
		Colour::Interpolate(*COLOUR_NORMAL, *primary, (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL), m_background->GetColourOffset());

		// Update background size.
		m_background->GetDimensions()->Set(*m_text->GetDimensions());
		m_background->GetDimensions()->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		Vector3::Multiply(*m_text->GetDimensions(), *m_background->GetDimensions(), m_background->GetDimensions());
		m_background->GetDimensions()->Scale(2.0f * m_text->GetScale());
		m_background->GetPosition()->Set(*m_text->GetPosition());
	}
}
