#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/Uis.hpp"
#include "UiInputDelay.hpp"
#include "UiObject.hpp"

namespace acid
{
	class ACID_EXPORT IUiGrabber
	{
	public:
		virtual ~IUiGrabber() = default;

		virtual int32_t GetCurrent(Text *object) = 0;

		virtual std::string GetValue(const int32_t &value) = 0;
	};

	class ACID_EXPORT UiGrabberJoystick :
		public IUiGrabber
	{
	private:
		JoystickPort m_joystick;
	public:
		UiGrabberJoystick(const JoystickPort &joystick);

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
		static const float CHANGE_TIME;
		static const float FONT_SIZE;
		static const Vector2 DIMENSION;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;

		Text *m_text;
		Gui *m_background;

		IUiGrabber *m_grabber;

		std::string m_prefix;
		int32_t m_value;

		UiInputDelay *m_inputDelay;
		int32_t m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		UiInputGrabber(UiObject *parent, const Vector3 &position, const std::string &prefix, const int32_t &value, IUiGrabber *grabber);

		~UiInputGrabber();

		void UpdateObject() override;

		bool OnActionMouse(const MouseButton &button) override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		int32_t GetValue() const { return m_value; }

		void SetValue(const int32_t &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
