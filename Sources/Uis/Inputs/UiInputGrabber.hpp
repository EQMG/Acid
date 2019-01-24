#pragma once

#include <functional>
#include "Audio/Sound.hpp"
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
		class ACID_EXPORT Grabber
		{
		public:
			Grabber() = default;

			~Grabber() = default;

			virtual int32_t GetCurrent(Text *object) = 0;

			virtual std::string GetValue(const int32_t &value) = 0;
		};

		class ACID_EXPORT GrabberJoystick :
			public Grabber
		{
		private:
			JoystickPort m_joystick;
		public:
			explicit GrabberJoystick(const JoystickPort &joystick);

			int32_t GetCurrent(Text *object) override;

			std::string GetValue(const int32_t &value) override;
		};

		class ACID_EXPORT GrabberKeyboard :
			public Grabber
		{
		public:
			int32_t GetCurrent(Text *object) override;

			std::string GetValue(const int32_t &value) override;
		};

		class ACID_EXPORT GrabberMouse :
			public Grabber
		{
		public:
			int32_t GetCurrent(Text *object) override;

			std::string GetValue(const int32_t &value) override;
		};

		UiInputGrabber(UiObject *parent, const std::string &prefix, const int32_t &value, Grabber *grabber,
			const UiBound &rectangle = UiBound(Vector3::Zero, UiBound::Centre, true, true, Vector2(0.36f, 0.05f)), const Colour &primaryColour = Colour("#171717"));

		void UpdateObject() override;

		const std::string &GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		const int32_t &GetValue() const { return m_value; }

		void SetValue(const int32_t &value);

		Delegate<void(UiInputGrabber *, int32_t)> &GetOnGrabbed() { return m_onGrabbed; }
	private:
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		std::unique_ptr<Grabber> m_grabber;

		std::string m_prefix;
		int32_t m_value;

		int32_t m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		Delegate<void(UiInputGrabber *, int32_t)> m_onGrabbed;
	};
}
