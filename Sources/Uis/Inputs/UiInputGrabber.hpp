#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Devices/Joysticks.hpp"
#include "Devices/Keyboard.hpp"
#include "Devices/Mouse.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/Uis.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputGrabber :
		public UiObject
	{
	public:
		UiInputGrabber(UiObject *parent, const std::string &prefix, const UiBound &rectangle, const Colour &primaryColour);

		~UiInputGrabber() = default;

		void UpdateObject() override;

		const std::string &GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);
	protected:
		void UpdateText();

		void Deselect();

		virtual std::string GetTextString() const = 0;

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		std::string m_prefix;

		int32_t m_lastKey;

		bool m_selected;
		bool m_mouseOver;
	};

	class ACID_EXPORT UiGrabberJoystick :
		public UiInputGrabber
	{
	public:
		UiGrabberJoystick(UiObject *parent, const std::string &prefix, const JoystickPort &port, const uint32_t &value, const UiBound &rectangle = UiBound(Vector3::Zero,
			UiReference::Centre, UiAspect::Position | UiAspect::Dimensions, Vector2(0.36f, 0.05f)), const Colour &primaryColour = Colour("#171717"));

		const JoystickPort &GetPort() const { return m_port; }

		void SetPort(const JoystickPort &port) { m_port = port; }

		const uint32_t &GetValue() const { return m_value; }

		void SetValue(const uint32_t &value)
		{
			m_value = value;
			UpdateText();
		}

		Delegate<void(UiGrabberJoystick *, JoystickPort, uint32_t)> &GetOnGrabbed() { return m_onGrabbed; }
	protected:
		std::string GetTextString() const override
		{
			return String::To(m_value);
		}
	private:
		JoystickPort m_port;
		uint32_t m_value;
		Delegate<void(UiGrabberJoystick *, JoystickPort, uint32_t)> m_onGrabbed;
	};

	class ACID_EXPORT UiGrabberKeyboard :
		public UiInputGrabber
	{
	public:
		UiGrabberKeyboard(UiObject *parent, const std::string &prefix, const Key &value, const UiBound &rectangle = UiBound(Vector3::Zero,
			UiReference::Centre, UiAspect::Position | UiAspect::Dimensions, Vector2(0.36f, 0.05f)), const Colour &primaryColour = Colour("#171717"));

		const Key &GetValue() const { return m_value; }

		void SetValue(const Key &value)
		{
			m_value = value;
			UpdateText();
		}

		Delegate<void(UiGrabberKeyboard *, Key)> &GetOnGrabbed() { return m_onGrabbed; }
	protected:
		std::string GetTextString() const override
		{
			return Keyboard::ToString(m_value);
		}
	private:
		Key m_value;
		Delegate<void(UiGrabberKeyboard *, Key)> m_onGrabbed;
	};

	class ACID_EXPORT UiGrabberMouse :
		public UiInputGrabber
	{
	public:
		UiGrabberMouse(UiObject *parent, const std::string &prefix, const MouseButton &value, const UiBound &rectangle = UiBound(Vector3::Zero,
			UiReference::Centre, UiAspect::Position | UiAspect::Dimensions, Vector2(0.36f, 0.05f)), const Colour &primaryColour = Colour("#171717"));

		const MouseButton &GetValue() const { return m_value; }

		void SetValue(const MouseButton &value)
		{
			m_value = value;
			UpdateText();
		}

		Delegate<void(UiGrabberMouse *, MouseButton)> &GetOnGrabbed() { return m_onGrabbed; }
	protected:
		std::string GetTextString() const override
		{
			return String::To(static_cast<int32_t>(m_value));
		}
	private:
		MouseButton m_value;
		Delegate<void(UiGrabberMouse *, MouseButton)> m_onGrabbed;
	};
}
