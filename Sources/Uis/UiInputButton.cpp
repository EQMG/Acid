#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"
#include "Uis.hpp"

namespace acid
{
	const float UiInputButton::CHANGE_TIME = 0.1f;
	const float UiInputButton::FONT_SIZE = 1.7f;
	const Vector2 UiInputButton::DIMENSION = Vector2(0.36f, 0.05f);
	const float UiInputButton::SCALE_NORMAL = 1.0f;
	const float UiInputButton::SCALE_SELECTED = 1.1f;

	UiInputButton::UiInputButton(UiObject *parent, const Vector2 &position, const std::string &string) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(std::make_unique<Text>(this, UiBound(position, "Centre", true), FONT_SIZE, string, FontType::Resource("Fonts/ProximaNova", "Regular"), JUSTIFY_CENTRE, DIMENSION.m_x)),
		m_background(std::make_unique<Gui>(this, UiBound(position, "Centre", true, true, DIMENSION), Texture::Resource("Guis/Button.png"))),
		m_mouseOver(false)
	{
	}

	UiInputButton::~UiInputButton()
	{
	}

	void UiInputButton::UpdateObject()
	{
		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && m_mouseOver)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}

	bool UiInputButton::OnActionMouse(const MouseButton &button)
	{
		if (button == MOUSE_BUTTON_LEFT)
		{
			// TODO: Click sound.
		}

		return false;
	}
}
