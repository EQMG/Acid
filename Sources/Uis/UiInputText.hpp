#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "UiInputDelay.hpp"
#include "UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputText :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float FONT_SIZE;
		static const Vector2 DIMENSION;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;

		std::unique_ptr<Text> m_text;
		std::unique_ptr<Gui> m_background;

		std::string m_prefix;
		std::string m_value;
		int32_t m_maxLength;

		UiInputDelay m_inputDelay;
		int32_t m_lastKey;

		bool m_selected;
		bool m_mouseOver;

		std::function<void()> m_actionChange;
	public:
		UiInputText(UiObject *parent, const Vector3 &position, const std::string &prefix, const std::string &value, const int32_t &maxLength);

		~UiInputText();

		void UpdateObject() override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		std::string GetValue() const { return m_value; }

		void SetValue(const std::string &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
