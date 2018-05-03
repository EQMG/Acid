#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "UiInputDelay.hpp"
#include "UiObject.hpp"

namespace fl
{
	class FL_EXPORT UiInputText :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static const Colour *COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		std::string m_prefix;
		std::string m_value;

		UiInputDelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		UiInputText(UiObject *parent, const Vector3 &position, const std::string &prefix, const std::string &value, const FontJustify &justify);

		~UiInputText();

		void UpdateObject() override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
