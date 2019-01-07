#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/Uis.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT IUiGrabber
	{
	public:
		virtual int32_t GetCurrent(Text *object) = 0;

		virtual std::string GetValue(const int32_t &value) = 0;
	};

	class ACID_EXPORT UiGrabberJoystick :
		public IUiGrabber
	{
	private:
		JoystickPort m_joystick;
	public:
		explicit UiGrabberJoystick(const JoystickPort &joystick);

		int32_t GetCurrent(Text *object) override;

		std::string GetValue(const int32_t &value) override;
	};

	class ACID_EXPORT UiGrabberKeyboard :
		public IUiGrabber
	{
	public:
		int32_t GetCurrent(Text *object) override;

		std::string GetValue(const int32_t &value) override;
	};

	class ACID_EXPORT UiGrabberMouse :
		public IUiGrabber
	{
	public:
		int32_t GetCurrent(Text *object) override;

		std::string GetValue(const int32_t &value) override;
	};

	class ACID_EXPORT UiInputGrabber :
		public UiObject
	{
	private:
		static const Time CHANGE_TIME;
		static const float FONT_SIZE;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		std::unique_ptr<IUiGrabber> m_grabber;

		std::string m_prefix;
		int32_t m_value;

		int32_t m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		Delegate<void(UiInputGrabber *, int32_t)> m_onGrabbed;
	public:
		UiInputGrabber(UiObject *parent, const std::string &prefix, const int32_t &value, IUiGrabber *grabber,
			const UiBound &rectangle = UiBound(Vector3::ZERO, UiBound::CENTRE, true, true, Vector2(0.36f, 0.05f)), const Colour &primaryColour = Colour("#171717"));

		void UpdateObject() override;

		const Gui *GetBackground() const { return m_background.get(); }

		const Text *GetText() const { return m_text.get(); }

		const std::string &GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		const int32_t &GetValue() const { return m_value; }

		void SetValue(const int32_t &value);

		Delegate<void(UiInputGrabber *, int32_t)> &GetOnGrabbed() { return m_onGrabbed; }
	};
}
