#pragma once

#include <functional>
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Maths/Timer.hpp"
#include "UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputSlider :
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
		std::unique_ptr<Gui> m_slider;

		std::string m_prefix;
		int32_t m_roundTo;
		bool m_updating;
		float m_progressMin;
		float m_progressMax;
		float m_value;

		bool m_mouseOver;

		bool m_hasChange;
		Timer m_timerChange;
		std::function<void()> m_actionChange;
	public:
		UiInputSlider(UiObject *parent, const Vector3 &position, const std::string &prefix, const int32_t &roundTo, const float &progressMin, const float &progressMax, const float &value);

		~UiInputSlider();

		void UpdateObject() override;

		std::string GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		float GetProgressMin() const { return m_progressMin; }

		void SetProgressMin(const float &progressMin) { m_progressMin = progressMin; }

		float GetProgressMax() const { return m_progressMax; }

		void SetProgressMax(const float &progressMax) { m_progressMax = progressMax; }

		float GetValue() const { return m_value; }

		void SetValue(const float &value);

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	private:
		void UpdateText();
	};
}
