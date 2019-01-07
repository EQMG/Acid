#pragma once

#include <functional>
#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Maths/Timer.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
	class ACID_EXPORT UiInputSlider :
		public UiObject
	{
	private:
		static const Time CHANGE_TIME;
		static const Time SLIDE_TIME;
		static const float FONT_SIZE;
		static const float SCALE_NORMAL;
		static const float SCALE_SELECTED;

		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Gui> m_slider;
		std::unique_ptr<Text> m_text;
		Sound m_soundClick;

		std::string m_prefix;
		bool m_updating;
		float m_value;
		float m_progressMin;
		float m_progressMax;
		int32_t m_roundTo;

		bool m_mouseOver;

		bool m_hasChange;
		Timer m_timerChange;
		Delegate<void(UiInputSlider *, float)> m_onSlide;
	public:
		UiInputSlider(UiObject *parent, const std::string &prefix, const float &value, const float &progressMin, const float &progressMax, const int32_t &roundTo,
			const UiBound &rectangle = UiBound(Vector3::ZERO, UiBound::CENTRE, true, true, Vector2(0.36f, 0.05f)),
			const Colour &primaryColour = Colour("#171717"), const Colour &secondaryColour = Colour("#202020"));

		void UpdateObject() override;

		const Gui *GetBackground() const { return m_background.get(); }

		const Gui *GetSlider() const { return m_slider.get(); }

		const Text *GetText() const { return m_text.get(); }

		const std::string &GetPrefix() const { return m_prefix; }

		void SetPrefix(const std::string &prefix);

		const float &GetProgressMin() const { return m_progressMin; }

		void SetProgressMin(const float &progressMin) { m_progressMin = progressMin; }

		const float &GetProgressMax() const { return m_progressMax; }

		void SetProgressMax(const float &progressMax) { m_progressMax = progressMax; }

		const float &GetValue() const { return m_value; }

		void SetValue(const float &value);

		Delegate<void(UiInputSlider *, float)> &GetOnSlide() { return m_onSlide; }
	private:
		void UpdateText();
	};
}
