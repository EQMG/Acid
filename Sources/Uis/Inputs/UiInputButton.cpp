#include "UiInputButton.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
	Colour UiInputButton::PrimaryColour = Colour("#3e4148");
	Colour UiInputButton::SecondaryColour = Colour("#161618");
	Colour UiInputButton::AccentColour = Colour("#215d9c");
	Colour UiInputButton::SelectedColour = Colour("#1c5087");

	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const float FONT_SIZE = 1.4f;

	UiInputButton::UiInputButton(UiObject *parent, const std::string &string, const UiBound &rectangle) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button_Filled.png"), AccentColour)),
		m_text(std::make_unique<Text>(this, UiBound::Centre, FONT_SIZE, string,
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_mouseOver(false)
	{
		GetRectangle().SetDimensions(SIZE);
		m_background->SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
	}

	void UiInputButton::UpdateObject()
	{
		if (m_background->IsSelected() && Uis::Get()->WasDown(MouseButton::Left))
		{
			if (!m_soundClick.IsPlaying())
			{
				m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
				m_soundClick.Play();
			}

			CancelEvent(MouseButton::Left);
		}

		if (m_background->IsSelected() && !m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), SelectedColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), AccentColour, SLIDE_TIME);
			m_mouseOver = false;
		}
	}
}
