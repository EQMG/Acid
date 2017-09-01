#pragma once

#include <functional>

#include "../maths/timer.hpp"
#include "../visual/driverslide.hpp"

#include "uis.hpp"
#include "uiobject.hpp"
#include "uialign.hpp"

namespace Flounder
{
	class inputslider :
		public UiObject
	{
	private:
		static const float CHANGE_TIME;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;
		static Colour *const COLOUR_NORMAL;

		Text *m_text;
		Gui *m_background;
		Gui *m_slider;

		bool m_updating;
		float m_progressMin;
		float m_progressMax;
		float m_value;

		bool m_mouseOver;

		bool m_hasChange;
		Timer *m_timerChange;
		std::function<void()> m_actionChange;
	public:
		inputslider(UiObject *parent, const Vector2 &position, const std::string &string, const float &progressMin, const float &progressMax, const float &value, const uialign &align);

		~inputslider();

		void UpdateObject() override;

		std::string getText() const { return m_text->GetText(); }

		void setText(const std::string &string) const { m_text->setText(string); }

		float getProgressMin() const { return m_progressMin; }

		void setProgressMin(const float &progressMin) { m_progressMin = progressMin; }

		float getProgressMax() const { return m_progressMax; }

		void setProgressMax(const float &progressMax) { m_progressMax = progressMax; }

		float getValue() const { return m_value; }

		void setValue(const float &value) { m_value = (value - m_progressMin) / (m_progressMax - m_progressMin); }

		void setActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
