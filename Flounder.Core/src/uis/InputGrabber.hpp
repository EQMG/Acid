#pragma once

#include <functional>

#include "../devices/Keyboard.hpp"
#include "../visual/driverslide.hpp"

#include "uis.hpp"
#include "uiobject.hpp"
#include "uialign.hpp"
#include "inputdelay.hpp"

namespace Flounder
{
	class igrabber
	{
	public:
		virtual ~igrabber() = default;

		virtual int getCurrent(Text *object) = 0;

		virtual std::string getValue(const int &value) = 0;
	};

	class grabberjoystick :
		public igrabber
	{
	private:
		int m_joystick;
	public:
		grabberjoystick(const int &joystick);

		int getCurrent(Text *object) override;

		std::string getValue(const int &value) override;
	};

	class grabberkeyboard :
		public igrabber
	{
	public:
		int getCurrent(Text *object) override;

		std::string getValue(const int &value) override;
	};

	class grabbermouse :
		public igrabber
	{
	public:
		int getCurrent(Text *object) override;

		std::string getValue(const int &value) override;
	};

	class inputgrabber :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static Colour *const COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		igrabber *m_grabber;

		std::string m_prefix;
		int m_value;

		inputdelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		inputgrabber(UiObject *parent, const Vector2 &position, const std::string &prefix, const int &value, igrabber *grabber, const uialign &align);

		~inputgrabber();

		void UpdateObject() override;

		std::string getPrefix() const { return m_prefix; }

		void setPrefix(const std::string &prefix);

		int getValue() const { return m_value; }

		void setValue(const int &value);

		void setActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
