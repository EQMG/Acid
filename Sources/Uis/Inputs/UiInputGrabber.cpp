#include "UiInputGrabber.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "UiInputButton.hpp"
#include "Uis/Uis.hpp"

namespace acid
{
UiInputGrabber::UiInputGrabber(UiObject* parent, const std::string& title, const UiBound& rectangle)
	: UiObject(parent, rectangle), m_background(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	  m_textTitle(this, UiBound(Vector2(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions), UiInputButton::FontSize, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
				  Colour::White),
	  m_textValue(this, UiBound(Vector2(2.5f * UiInputButton::Padding.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions), UiInputButton::FontSize, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	  m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f), m_title(title), m_lastKey(0), m_selected(false), m_mouseOver(false)
{
	GetRectangle().SetDimensions(UiInputButton::Size);
	m_background.SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
}

void UiInputGrabber::UpdateObject()
{
	if(Uis::Get()->WasDown(MouseButton::Left))
		{
			if(m_background.IsSelected())
				{
					SetSelected(true);
					CancelEvent(MouseButton::Left);
				}
			else if(m_selected)
				{
					SetSelected(false);
					CancelEvent(MouseButton::Left);
				}
		}

	if(!m_selected)
		{
			if(m_background.IsSelected() && !m_mouseOver)
				{
					m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime);
					m_mouseOver = true;
				}
			else if(!m_background.IsSelected() && m_mouseOver)
				{
					m_background.SetColourDriver<DriverSlide<Colour>>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime);
					m_mouseOver = false;
				}
		}
}

void UiInputGrabber::SetSelected(const bool& selected)
{
	if(!m_soundClick.IsPlaying())
		{
			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

	m_selected = selected;
	m_mouseOver = true;
}

void UiInputGrabber::SetTitle(const std::string& title)
{
	m_title = title;
	m_textTitle.SetString(m_title);
}

void UiInputGrabber::UpdateText()
{
	m_textValue.SetString(GetTextString());
}

UiGrabberJoystick::UiGrabberJoystick(UiObject* parent, const std::string& title, const uint32_t& port, const uint32_t& value, const UiBound& rectangle) : UiInputGrabber(parent, title, rectangle), m_port(port), m_value(value)
{
	UpdateText();

	Joysticks::Get()->GetOnButton() += [this](uint32_t port, uint32_t button, InputAction action) {
		if(!m_selected || port != m_port)
			{
				return;
			}

		m_value = button;
		m_onGrabbed(this, m_port, m_value);
		SetSelected(false);
		UpdateText();
	};
}

UiGrabberKeyboard::UiGrabberKeyboard(UiObject* parent, const std::string& title, const Key& value, const UiBound& rectangle) : UiInputGrabber(parent, title, rectangle), m_value(value)
{
	UpdateText();

	Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, BitMask<InputMod> mods) {
		if(!m_selected)
			{
				return;
			}

		m_value = key;
		m_onGrabbed(this, m_value);
		SetSelected(false);
		UpdateText();
	};
}

UiGrabberMouse::UiGrabberMouse(UiObject* parent, const std::string& title, const MouseButton& value, const UiBound& rectangle) : UiInputGrabber(parent, title, rectangle), m_value(value)
{
	UpdateText();

	Mouse::Get()->GetOnButton() += [this](MouseButton button, InputAction action, BitMask<InputMod> mods) {
		if(!m_selected)
			{
				return;
			}

		if(button == MouseButton::Left)
			{
				if(!m_background.IsSelected() || m_soundClick.IsPlaying())
					{
						return;
					}

				CancelEvent(MouseButton::Left);
			}

		m_value = button;
		m_onGrabbed(this, m_value);
		SetSelected(false);
		UpdateText();
	};
}
}
