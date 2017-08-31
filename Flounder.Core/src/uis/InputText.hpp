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
	class inputtext :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static Colour *const COLOUR_NORMAL;

		text *m_text;
		gui *m_background;

		std::string m_prefix;
		std::string m_value;

		inputdelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		inputtext(UiObject *parent, const Vector2 &position, const std::string &prefix, const std::string &value, const uialign &align);

		~inputtext();

		void UpdateObject() override;

		std::string getPrefix() const { return m_prefix; }

		void setPrefix(const std::string &prefix);

		std::string getValue() const { return m_value; }

		void setValue(const std::string &value);

		void setActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
