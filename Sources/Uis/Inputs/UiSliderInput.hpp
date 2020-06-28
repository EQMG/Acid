#pragma once

#include "Fonts/Text.hpp"
#include "Guis/Gui.hpp"
#include "Uis/UiObject.hpp"
#include "UiButtonInput.hpp"

namespace acid {
class ACID_EXPORT UiSliderInput : public UiObject {
public:
	UiSliderInput();

	void UpdateObject() override;

	float GetValueMin() const { return valueMin; }
	void SetValueMin(float valueMin) { this->valueMin = valueMin; }

	float GetValueMax() const { return valueMax; }
	void SetValueMax(float valueMax) { this->valueMax = valueMax; }

	const std::string &GetTitle() const { return textTitle.GetString(); }
	void SetTitle(const std::string &string) { textTitle.SetString(string); }

	float GetValue() const { return value; }
	void SetValue(float value);

	float GetProgress() const { return progress; }

	int32_t GetRoundTo() const { return roundTo; }
	void SetRoundTo(int32_t roundTo) { this->roundTo = roundTo; }

	/**
	 * Called when this value of the input changes.
	 * @return The delegate.
	 */
	rocket::signal<void(float)> &OnValue() { return onValue; }

private:
	void UpdateProgress();

	Gui slider;
	Gui background;
	Text textTitle;
	Text textValue;

	float value = 0.5f;
	float valueMin = 0.0f, valueMax = 1.0f;
	float progress = 0.0f;
	int32_t roundTo = 2;

	bool updating = false;
	bool mouseOver = false;

	rocket::signal<void(float)> onValue;
};
}
