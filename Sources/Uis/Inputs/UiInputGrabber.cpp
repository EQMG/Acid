#include "UiInputGrabber.hpp"

#include "Maths/Visual/DriverSlide.hpp"

namespace acid
{
	static const Time SLIDE_TIME = Time::Seconds(0.2f);
	static const Vector2 SIZE = Vector2(0.3f, 0.045f);
	static const Vector2 PADDING = Vector2(0.01f, 0.07f);
	static const float FONT_SIZE = 1.4f;

	UiInputGrabber::UiInputGrabber(UiObject *parent, const std::string &title, const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_slider(std::make_unique<Gui>(m_background.get(), UiBound(Vector2(0.5f, 0.5f), UiReference::Centre, UiAspect::Position | UiAspect::Scale, 1.0f - (2.0f * PADDING)),
			Texture::Create("Guis/Button.png"), secondaryColour)),
		m_textTitle(std::make_unique<Text>(this, UiBound(Vector2(1.0f - (2.0f * PADDING.m_x), 0.5f), UiReference::CentreRight, UiAspect::Position | UiAspect::Dimensions),
			FONT_SIZE, title, FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_textValue(std::make_unique<Text>(this, UiBound(Vector2(2.0f * PADDING.m_x, 0.5f), UiReference::CentreLeft, UiAspect::Position | UiAspect::Dimensions),
		    FONT_SIZE, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, SIZE.m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_title(title),
		m_lastKey(0),
		m_selected(false),
		m_primaryColour(primaryColour),
		m_mouseOver(false)
	{
		GetRectangle().SetDimensions(SIZE);
	}

	void UiInputGrabber::UpdateObject()
	{
		// Click updates.
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 1.4f * m_primaryColour, SLIDE_TIME);
			m_selected = true;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();

			CancelEvent(MouseButton::Left);
		}
		else if (Uis::Get()->WasDown(MouseButton::Left) && m_selected)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_selected = false;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		// Mouse over updates.
		if (m_background->IsSelected() && !m_mouseOver && !m_selected)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), 1.4f * m_primaryColour, SLIDE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver && !m_selected)
		{
			m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
			m_mouseOver = false;
		}
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

	void UiInputGrabber::Deselect()
	{
		m_background->SetColourDriver<DriverSlide<Colour>>(m_background->GetColourOffset(), m_primaryColour, SLIDE_TIME);
		CancelEvent(MouseButton::Left);
		m_selected = false;

		m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
		m_soundClick.Play();
	}

	UiGrabberJoystick::UiGrabberJoystick(UiObject *parent, const std::string &title, const uint32_t &port,
		const uint32_t &value, const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiInputGrabber(parent, title, rectangle, primaryColour, secondaryColour),
		m_port(port),
		m_value(value),
		m_onGrabbed(Delegate<void(UiGrabberJoystick *, uint32_t, uint32_t)>())
	{
		Joysticks::Get()->GetOnButton() += [this](uint32_t port, uint32_t button, InputAction action) {
			if (!m_selected || port != m_port)
			{
				return;
			}

			m_value = button;
			m_onGrabbed(this, m_port, m_value);
			UpdateText();
			Deselect();
		};

		UpdateText();
	}

	UiGrabberKeyboard::UiGrabberKeyboard(UiObject *parent, const std::string &title, const Key &value,
		const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiInputGrabber(parent, title, rectangle, primaryColour, secondaryColour),
		m_value(value),
		m_onGrabbed(Delegate<void(UiGrabberKeyboard *, Key)>())
	{
		Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, bitmask<InputMod> mods) {
			if (!m_selected)
			{
				return;
			}

			m_value = key;
			m_onGrabbed(this, m_value);
			UpdateText();
			Deselect();
		};

		UpdateText();
	}

	UiGrabberMouse::UiGrabberMouse(UiObject *parent, const std::string &title, const MouseButton &value,
		const UiBound &rectangle, const Colour &primaryColour, const Colour &secondaryColour) :
		UiInputGrabber(parent, title, rectangle, primaryColour, secondaryColour),
		m_value(value),
		m_onGrabbed(Delegate<void(UiGrabberMouse *, MouseButton)>())
	{
		Mouse::Get()->GetOnButton() += [this](MouseButton button, InputAction action, bitmask<InputMod> mods) {
			if (!m_selected)
			{
				return;
			}

			if (button == MouseButton::Left)
			{
				CancelEvent(MouseButton::Left);

				if (m_soundClick.IsPlaying())
				{
					return;
				}
			}

			m_value = button;
			m_onGrabbed(this, m_value);
			UpdateText();
			Deselect();
		};

		UpdateText();
	}
}
