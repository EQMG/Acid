﻿#pragma once

#include <functional>
#include "../Fonts/Text.hpp"
#include "../Guis/Gui.hpp"
#include "../Maths/Timer.hpp"
#include "Uis.hpp"
#include "UiObject.hpp"

namespace Flounder
{
	class InputSlider :
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
		InputSlider(UiObject *parent, const Vector3 &position, const std::string &string, const float &progressMin, const float &progressMax, const float &value, const Justify &justify);

		~InputSlider();

		void UpdateObject() override;

		std::string GetText() const { return m_text->GetText(); }

		void SetText(const std::string &string) const { m_text->SetText(string); }

		float GetProgressMin() const { return m_progressMin; }

		void SetProgressMin(const float &progressMin) { m_progressMin = progressMin; }

		float GetProgressMax() const { return m_progressMax; }

		void SetProgressMax(const float &progressMax) { m_progressMax = progressMax; }

		float GetValue() const { return m_value; }

		void SetValue(const float &value) { m_value = (value - m_progressMin) / (m_progressMax - m_progressMin); }

		void SetActionChange(std::function<void()> action) { m_actionChange = action; }
	};
}
