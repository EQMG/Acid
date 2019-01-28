#include "UiInputGrabber.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "UiInputButton.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.1f);
	static const Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const Vector2 PADDING = Vector2(0.01f, 0.07f);
	static const float FONT_SIZE = 1.4f;

	UiInputGrabber::UiInputGrabber(UiObject *parent, const std::string &title, const UiBound &rectangle) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), UiInputButton::PrimaryColour)),
		m_textTitle(std::make_unique<Text>(this, UiBound(Vector2(1.0f - (2.5f * PADDING.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_textValue(std::make_unique<Text>(this, UiBound(Vector2(2.5f * PADDING.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions),
		    FONT_SIZE, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_title(title),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false)
	{
		GetRectangle().SetDimensions(SIZE);
	}

	void UiInputGrabber::UpdateObject()
	{
		if (Uis::Get()->WasDown(MouseButton::Left))
		{
			if (m_background->IsSelected())
			{
				SetSelected(true);
				CancelEvent(MouseButton::Left);
			}
			else if (m_selected)
			{
				SetSelected(false);
				CancelEvent(MouseButton::Left);
			}
		}

		if (!m_selected)
		{
			if (m_background->IsSelected() && !m_mouseOver)
			{
				m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), UiInputButton::SelectedColour, SLIDE_TIME);
				m_mouseOver = true;
			}
			else if (!m_background->IsSelected() && m_mouseOver)
			{
				m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), UiInputButton::PrimaryColour, SLIDE_TIME);
				m_mouseOver = false;
			}
		}
	}

	void UiInputGrabber::SetSelected(const bool &selected)
	{
		if (!m_soundClick.IsPlaying())
		{
			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		m_selected = selected;
		m_mouseOver = true;
	}

	void UiInputGrabber::SetTitle(const std::string &title)
	{
		m_title = title;
		m_textTitle->SetString(m_title);
	}

	void UiInputGrabber::UpdateText()
	{
		m_textValue->SetString(GetTextString());
	}

	UiGrabberJoystick::UiGrabberJoystick(UiObject *parent, const std::string &title, const uint32_t &port,
		const uint32_t &value, const UiBound &rectangle) :
		UiInputGrabber(parent, title, rectangle),
		m_port(port),
		m_value(value),
		m_onGrabbed(Delegate<void(UiGrabberJoystick *, uint32_t, uint32_t)>())
	{
		UpdateText();

		Joysticks::Get()->GetOnButton() += [this](uint32_t port, uint32_t button, InputAction action) {
			if (!m_selected || port != m_port)
			{
				return;
			}

			m_value = button;
			m_onGrabbed(this, m_port, m_value);
			SetSelected(false);
			UpdateText();
		};
	}

	UiGrabberKeyboard::UiGrabberKeyboard(UiObject *parent, const std::string &title, const Key &value,
		const UiBound &rectangle) :
		UiInputGrabber(parent, title, rectangle),
		m_value(value),
		m_onGrabbed(Delegate<void(UiGrabberKeyboard *, Key)>())
	{
		UpdateText();

		Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, bitmask<InputMod> mods) {
			if (!m_selected)
			{
				return;
			}

			m_value = key;
			m_onGrabbed(this, m_value);
			SetSelected(false);
			UpdateText();
		};
	}

	UiGrabberMouse::UiGrabberMouse(UiObject *parent, const std::string &title, const MouseButton &value,
		const UiBound &rectangle) :
		UiInputGrabber(parent, title, rectangle),
		m_value(value),
		m_onGrabbed(Delegate<void(UiGrabberMouse *, MouseButton)>())
	{
		UpdateText();

		Mouse::Get()->GetOnButton() += [this](MouseButton button, InputAction action, bitmask<InputMod> mods) {
			if (!m_selected)
			{
				return;
			}

			if (button == MouseButton::Left)
			{
				if (!m_background->IsSelected() || m_soundClick.IsPlaying())
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
