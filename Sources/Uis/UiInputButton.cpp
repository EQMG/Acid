#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	const float UiInputButton::CHANGE_TIME = 0.1f;
	const float UiInputButton::SCALE_NORMAL = 1.6f;
	const float UiInputButton::SCALE_SELECTED = 1.8f;
	const Colour UiInputButton::COLOUR_NORMAL = Colour("#000000");

	UiInputButton::UiInputButton(UiObject *parent, const Vector2 &position, const std::string &string, const TextJustify &justify) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(new Text(this, UiBound(position, "Centre", true), SCALE_NORMAL, string, FontType::Resource("Fonts/ProximaNova", "Regular"), justify, 0.36f)),
		m_background(new Gui(this, UiBound(position, "Centre", true, true, Vector2(0.36f, 0.05f)), Texture::Resource("Guis/Button.png"))),
		m_mouseOver(false)
	{
	}

	UiInputButton::~UiInputButton()
	{
		delete m_text;
		delete m_background;
	}

	void UiInputButton::UpdateObject()
	{
		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver)
		{
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && m_mouseOver)
		{
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}

		// Update the background colour.
		auto primary = Scenes::Get()->GetScene()->GetUiColour();
		m_background->SetColourOffset(COLOUR_NORMAL.Interpolate(*primary, (m_text->GetScale() - SCALE_NORMAL) / (SCALE_SELECTED - SCALE_NORMAL)));

		// Update background size.
		//m_background->GetRectangle()->m_dimensions->Set(*m_text->GetRectangle()->m_dimensions);
		//m_background->GetRectangle()->m_dimensions->m_y = 0.5f * static_cast<float>(m_text->GetFontType()->GetMetadata()->GetMaxSizeY());
		//Vector2::Multiply(*m_text->GetRectangle()->m_dimensions, *m_background->GetRectangle()->m_dimensions, m_background->GetRectangle()->m_dimensions);
		//m_background->GetRectangle()->m_dimensions->Scale(2.0f * m_text->GetScale());
		//m_background->GetRectangle()->m_position->Set(*m_text->GetRectangle()->m_position);
	}
}
