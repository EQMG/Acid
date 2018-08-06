#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "UiInputDelay.hpp"
#include "UiObject.hpp"

namespace acid
{
	class ACID_EXPORT IUiGrabber
	{
	public:
		virtual ~IUiGrabber() = default;

		virtual int GetCurrent(Text *object) = 0;

		virtual std::string GetValue(const int &value) = 0;
	};

	class ACID_EXPORT UiGrabberJoystick :
		public IUiGrabber
	{
	private:
		JoystickPort m_joystick;
	public:
		UiGrabberJoystick(const JoystickPort &joystick);

		int GetCurrent(Text *object) override;

		std::string GetValue(const int &value) override;
	};

	class ACID_EXPORT UiGrabberKeyboard :
		public IUiGrabber
	{
	public:
		int GetCurrent(Text *object) override;

		std::string GetValue(const int &value) override;
	};

	class ACID_EXPORT UiGrabberMouse :
		public IUiGrabber
	{
	public:
		int GetCurrent(Text *object) override;

		std::string GetValue(const int &value) override;
	};

	class ACID_EXPORT UiInputGrabber :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static const Colour COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		IUiGrabber *m_grabber;

		std::string m_prefix;
		int m_value;

		UiInputDelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		UiInputGrabber(UiObject *parent, const Vector3 &position, const std::string &prefix, const int &value, IUiGrabber *grabber, const TextJustify &justify);

		~UiInputGrabber();

		void UpdateObject() override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		int GetValue() const { return m_value; }

		void SetValue(const int &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
