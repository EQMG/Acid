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
	public:
		UiInputSlider(UiObject *parent, const std::string &title, const float &value, const float &progressMin, const float &progressMax, const int32_t &roundTo = 2,
			const UiBound &rectangle = UiBound(Vector3::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions),
			const Colour &primaryColour = Colour("#3e4148"), const Colour &secondaryColour = Colour("#161618"));

		void UpdateObject() override;

		const std::string &GetTitle() const { return m_title; }

		void SetTitle(const std::string &title);

		const float &GetProgressMin() const { return m_progressMin; }

		void SetProgressMin(const float &progressMin) { m_progressMin = progressMin; }

		const float &GetProgressMax() const { return m_progressMax; }

		void SetProgressMax(const float &progressMax) { m_progressMax = progressMax; }

		const float &GetValue() const { return m_value; }

		void SetValue(const float &value);

		Delegate<void(UiInputSlider *, float)> &GetOnSlide() { return m_onSlide; }
	private:
		void UpdateText();

		std::unique_ptr<Gui> m_slider;
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<Text> m_textTitle;
		std::unique_ptr<Text> m_textValue;
		Sound m_soundClick;

		std::string m_title;
		bool m_updating;
		float m_value;
		float m_progressMin;
		float m_progressMax;
		int32_t m_roundTo;

		Colour m_primaryColour;
		bool m_mouseOver;

		bool m_hasChange;
		Timer m_timerChange;
		Delegate<void(UiInputSlider *, float)> m_onSlide;
	};
}
