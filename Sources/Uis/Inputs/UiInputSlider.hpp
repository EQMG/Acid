#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiInputButton.hpp"

namespace acid {
class ACID_EXPORT UiInputSlider : public UiObject {
public:
	UiInputSlider();

	void UpdateObject() override;

	float GetValueMin() const { return m_valueMin; }
	void SetValueMin(float valueMin) { m_valueMin = valueMin; }

	float GetValueMax() const { return m_valueMax; }
	void SetValueMax(float valueMax) { m_valueMax = valueMax; }

	const std::string &GetTitle() const { return m_textTitle.GetString(); }
	void SetTitle(const std::string &string) { m_textTitle.SetString(string); }

	float GetValue() const { return m_value; }
	void SetValue(float value);

	float GetProgress() const { return m_progress; }

	int32_t GetRoundTo() const { return m_roundTo; }
	void SetRoundTo(int32_t roundTo) { m_roundTo = roundTo; }

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

	float m_value = 0.5f;
	float m_valueMin = 0.0f;
	float m_valueMax = 1.0f;
	float m_progress = 0.0f;
	int32_t m_roundTo = 2;

	bool m_updating = false;
	bool m_mouseOver = false;

	Delegate<void(float)> m_onValue;
};
}
