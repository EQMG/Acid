#include "UiInputGrabber.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Uis/Uis.hpp"
#include "UiInputButton.hpp"

namespace acid
{
UiInputGrabber::UiInputGrabber(UiObject *parent, const std::string &title, const UiBound &rectangle) :
	UiObject(parent, rectangle),
	m_background(this, UiBound::Maximum, Image2d::Create("Guis/Button.png"), UiInputButton::PrimaryColour),
	m_textTitle(this, UiBound(Vector2f(1.0f - (2.5f * UiInputButton::Padding.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Size),
		UiInputButton::FontSize, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	m_textValue(this, UiBound(Vector2f(2.5f * UiInputButton::Padding.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Size), UiInputButton::FontSize, "",
		FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White),
	m_soundClick("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f),
	m_lastKey(0),
	m_updating(false),
	m_mouseOver(false)
{
	GetRectangle().SetSize(UiInputButton::Size);
	m_background.SetNinePatches(Vector4f(0.125f, 0.125f, 0.75f, 0.75f));

	OnSelected() += [this](bool selected)
	{
		Mouse::Get()->SetCursor(selected ? CursorStandard::Hand : CursorStandard::Arrow);
	};
}

void UiInputGrabber::UpdateObject()
{
	if (Uis::Get()->WasDown(MouseButton::Left))
	{
		if (m_background.IsSelected())
		{
			SetUpdating(true);
			CancelEvent(MouseButton::Left);
		}
		else if (m_updating)
		{
			SetUpdating(false);
			CancelEvent(MouseButton::Left);
		}
	}

	if (!m_updating)
	{
		if (m_background.IsSelected() && !m_mouseOver)
		{
			m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), UiInputButton::SelectedColour, UiInputButton::SlideTime));
			m_mouseOver = true;
		}
		else if (!m_background.IsSelected() && m_mouseOver)
		{
			m_background.SetColourDriver(new DriverSlide<Colour>(m_background.GetColourOffset(), UiInputButton::PrimaryColour, UiInputButton::SlideTime));
			m_mouseOver = false;
		}
	}
}

void UiInputGrabber::SetUpdating(const bool &updating)
{
	if (!m_soundClick.IsPlaying())
	{
		m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
		m_soundClick.Play();
	}

	m_updating = updating;
	m_mouseOver = true;
}

void UiInputGrabber::UpdateValue()
{
	m_textValue.SetString(GetTextString());
}

UiGrabberJoystick::UiGrabberJoystick(UiObject *parent, const std::string &title, const uint32_t &port, const uint32_t &value, const UiBound &rectangle) :
	UiInputGrabber(parent, title, rectangle),
	m_port(port),
	m_value(value)
{
	UpdateValue();

	Joysticks::Get()->OnButton() += [this](uint32_t port, uint32_t button, InputAction action)
	{
		if (!m_updating || port != m_port)
		{
			return;
		}

		m_value = button;
		m_onValue(m_port, m_value);
		SetUpdating(false);
		UpdateValue();
	};
}

void UiGrabberJoystick::SetValue(const uint32_t &value)
{
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

UiGrabberKeyboard::UiGrabberKeyboard(UiObject *parent, const std::string &title, const Key &value, const UiBound &rectangle) :
	UiInputGrabber(parent, title, rectangle),
	m_value(value)
{
	UpdateValue();

	Keyboard::Get()->OnKey() += [this](Key key, InputAction action, BitMask<InputMod> mods)
	{
		if (!m_updating)
		{
			return;
		}

		m_value = key;
		m_onValue(m_value);
		SetUpdating(false);
		UpdateValue();
	};
}

void UiGrabberKeyboard::SetValue(const Key &value)
{
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}

UiGrabberMouse::UiGrabberMouse(UiObject *parent, const std::string &title, const MouseButton &value, const UiBound &rectangle) :
	UiInputGrabber(parent, title, rectangle),
	m_value(value)
{
	UpdateValue();

	Mouse::Get()->OnButton() += [this](MouseButton button, InputAction action, BitMask<InputMod> mods)
	{
		if (!m_updating)
		{
			return;
		}

		if (button == MouseButton::Left)
		{
			if (!m_background.IsSelected() || m_soundClick.IsPlaying())
			{
				return;
			}

			CancelEvent(MouseButton::Left);
		}

		m_value = button;
		m_onValue(m_value);
		SetUpdating(false);
		UpdateValue();
	};
}

void UiGrabberMouse::SetValue(const MouseButton &value)
{
	m_value = value;
	UpdateValue();
	//m_onValue(m_value);
}
}
