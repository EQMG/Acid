#pragma once

#include "Audio/Sound.hpp"
#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Maths/Timer.hpp"
#include "Uis/UiObject.hpp"

namespace acid
{
class ACID_EXPORT UiInputSlider : public UiObject
{
  public:
	UiInputSlider(UiObject* parent, const std::string& title, const float& value, const float& valueMin, const float& valueMax, const int32_t& roundTo = 2, const UiBound& rectangle = UiBound(Vector2::Zero, UiReference::Centre, UiAspect::Position | UiAspect::Dimensions));

	void UpdateObject() override;

	const std::string& GetTitle() const
	{
		return m_title;
	}

	void SetTitle(const std::string& title);

	const float& GetValueMin() const
	{
		return m_valueMin;
	}

	void SetValueMin(const float& valueMin)
	{
		m_valueMin = valueMin;
	}

	const float& GetValueMax() const
	{
		return m_valueMax;
	}

	void SeValueMax(const float& valueMax)
	{
		m_valueMax = valueMax;
	}

	const float& GetValue() const
	{
		return m_value;
	}

	void SetValue(const float& value);

	const float& GetProgress() const
	{
		return m_progress;
	}

	Delegate<void(UiInputSlider*, float)>& GetOnSlide()
	{
		return m_onSlide;
	}

  private:
	void UpdateValueText();

	Gui m_slider;
	Gui m_background;
	Text m_textTitle;
	Text m_textValue;
	Sound m_soundClick;

	std::string m_title;
	bool m_updating;
	float m_value;
	float m_valueMin;
	float m_valueMax;
	float m_progress;
	int32_t m_roundTo;

	bool m_mouseOver;

	bool m_hasChange;
	Timer m_timerChange;
	Delegate<void(UiInputSlider*, float)> m_onSlide;
};
}
