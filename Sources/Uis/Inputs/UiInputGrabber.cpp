#include "UiInputGrabber.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	static const Time CHANGE_TIME = Time::Seconds(0.1f);
	static const float FONT_SIZE = 1.7f;
	static const float SCALE_NORMAL = 1.0f;
	static const float SCALE_SELECTED = 1.1f;

	UiInputGrabber::UiInputGrabber(UiObject *parent, const std::string &prefix, const UiBound &rectangle, const Colour &primaryColour) :
		UiObject(parent, rectangle),
		m_background(std::make_unique<Gui>(this, UiBound::Maximum, Texture::Create("Guis/Button.png"), primaryColour)),
		m_text(std::make_unique<Text>(this, UiBound::Centre, FONT_SIZE, "",
			FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, rectangle.GetDimensions().m_x, Colour::White)),
		m_soundClick(Sound("Sounds/Button1.ogg", Transform::Identity, Audio::Type::Effect, false, false, 0.9f)),
		m_prefix(prefix),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false)
	{
	}

	void UiInputGrabber::UpdateObject()
	{
		// Click updates.
		if (m_background->IsSelected() && GetAlpha() == 1.0f && Uis::Get()->WasDown(MouseButton::Left))
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_selected = true;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();

			CancelEvent(MouseButton::Left);
		}
		else if (Uis::Get()->WasDown(MouseButton::Left) && m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_selected = false;

			m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
			m_soundClick.Play();
		}

		// Mouse over updates.
		if (m_background->IsSelected() && !m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!m_background->IsSelected() && m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}

	void UiInputGrabber::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		UpdateText();
	}

	void UiInputGrabber::UpdateText()
	{
		m_text->SetString(m_prefix + GetTextString());
	}

	void UiInputGrabber::Deselect()
	{
		m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
		m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
		CancelEvent(MouseButton::Left);
		m_selected = false;

		m_soundClick.SetPitch(Maths::Random(0.7f, 0.9f));
		m_soundClick.Play();
	}

	UiGrabberJoystick::UiGrabberJoystick(UiObject *parent, const std::string &prefix, const uint32_t &port,
		const uint32_t &value, const UiBound &rectangle, const Colour &primaryColour) :
		UiInputGrabber(parent, prefix, rectangle, primaryColour),
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

	UiGrabberKeyboard::UiGrabberKeyboard(UiObject *parent, const std::string &prefix, const Key &value,
		const UiBound &rectangle, const Colour &primaryColour) :
		UiInputGrabber(parent, prefix, rectangle, primaryColour),
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

	UiGrabberMouse::UiGrabberMouse(UiObject *parent, const std::string &prefix, const MouseButton &value,
		const UiBound &rectangle, const Colour &primaryColour) :
		UiInputGrabber(parent, prefix, rectangle, primaryColour),
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
				return;
			}

			m_value = button;
			m_onGrabbed(this, m_value);
			UpdateText();
			Deselect();
		};

		UpdateText();
	}
}
