#pragma once

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
	UiInputSlider(UiObject *parent, const std::string &title, const float &value, const float &valueMin, const float &valueMax, const int32_t &roundTo = 2,
		const UiBound &rectangle = UiBound(Vector2f(0.0f, 0.0f), UiReference::Centre, UiAspect::Position | UiAspect::Size));

	void UpdateObject() override;

	const float &GetValueMin() const { return m_valueMin; }

	void SetValueMin(const float &valueMin) { m_valueMin = valueMin; }

	const float &GetValueMax() const { return m_valueMax; }

	void SetValueMax(const float &valueMax) { m_valueMax = valueMax; }

	const std::string &GetTitle() const { return m_textTitle.GetString(); }

	void SetTitle(const std::string &title) { m_textTitle.SetString(title); }

	const float &GetValue() const { return m_value; }
	
	void SetValue(const float &value);

	const float &GetProgress() const { return m_progress; }

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	Delegate<void(float)> &OnValue() { return m_onValue; }

private:
	void UpdateProgress();

	Gui m_slider;
	Gui m_background;
	Text m_textTitle;
	Text m_textValue;

	float m_value;
	float m_valueMin;
	float m_valueMax;
	float m_progress;
	int32_t m_roundTo;

	bool m_updating;
	bool m_mouseOver;

	bool m_hasChange;
	Timer m_timerChange;
	Delegate<void(float)> m_onValue;
};
}
