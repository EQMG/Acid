#pragma once

#include <functional>

#include "../devices/keyboard.h"
#include "../visual/driverslide.h"

#include "uis.h"
#include "uiobject.h"
#include "uialign.h"
#include "inputdelay.h"

namespace flounder
{
	class igrabber
	{
	public:
		virtual ~igrabber() = default;

		virtual int getCurrent(text *object) = 0;

		virtual std::string getValue(const int &value) = 0;
	};

	class grabbermouse :
		public igrabber
	{
	public:
		int getCurrent(text *object) override;

		std::string getValue(const int &value) override;
	};

	class grabberkeyboard :
		public igrabber
	{
	public:
		int getCurrent(text *object) override;

		std::string getValue(const int &value) override;
	};

	class inputgrabber :
		public uiobject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static colour *const COLOUR_NORMAL;

		text *m_text;
		gui *m_background;

		igrabber *m_grabber;

		std::string m_prefix;
		int m_value;

		inputdelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		inputgrabber(uiobject *parent, const vector2 &position, const std::string &prefix, const int &value, igrabber *grabber, const uialign &align);

		~inputgrabber();

		void updateObject() override;

		std::string getPrefix() const { return m_prefix; }

		void setPrefix(const std::string &prefix);

		int getValue() const { return m_value; }

		void setValue(const int &value);

		void setActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
