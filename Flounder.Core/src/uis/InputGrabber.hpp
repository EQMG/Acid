#pragma once

#include <functional>

#include "Uis.hpp"
#include "UiObject.hpp"
#include "UiAlign.hpp"
#include "InputDelay.hpp"

namespace Flounder
{
	class IGrabber
	{
	public:
		virtual ~IGrabber() = default;

		virtual int GetCurrent(Text *object) = 0;

		virtual std::string GetValue(const int &value) = 0;
	};

	class GrabberJoystick :
		public IGrabber
	{
	private:
		int m_joystick;
	public:
		GrabberJoystick(const int &joystick);

		int GetCurrent(Text *object) override;

		std::string GetValue(const int &value) override;
	};

	class GrabberKeyboard :
		public IGrabber
	{
	public:
		int GetCurrent(Text *object) override;

		std::string GetValue(const int &value) override;
	};

	class GrabberMouse :
		public IGrabber
	{
	public:
		int GetCurrent(Text *object) override;

		std::string GetValue(const int &value) override;
	};

	class InputGrabber :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static Colour *const COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		IGrabber *m_grabber;

		std::string m_prefix;
		int m_value;

		InputDelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		InputGrabber(UiObject *parent, const Vector2 &position, const std::string &prefix, const int &value, IGrabber *grabber, const UiAlign &align);

		~InputGrabber();

		void UpdateObject() override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		int GetValue() const { return m_value; }

		void SetValue(const int &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
