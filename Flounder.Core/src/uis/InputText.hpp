#pragma once

#include <functional>

#include "../devices/Keyboard.hpp"
#include "../visual/DriverSlide.hpp"

#include "Uis.hpp"
#include "UiObject.hpp"
#include "UiAlign.hpp"
#include "InputDelay.hpp"

namespace Flounder
{
	class InputText :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static Colour *const COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;

		std::string m_prefix;
		std::string m_value;

		InputDelay *m_inputDelay;
		int m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		InputText(UiObject *parent, const Vector2 &position, const std::string &prefix, const std::string &value, const UiAlign &align);

		~InputText();

		void UpdateObject() override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
