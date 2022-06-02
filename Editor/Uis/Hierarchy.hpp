#pragma once

#include <Uis/Inputs/UiButtonInput.hpp>
#include <Uis/Inputs/UiGrabberInput.hpp>
#include <Uis/Inputs/UiSliderInput.hpp>
#include <Uis/Inputs/UiTextInput.hpp>
//#include <Uis/Inputs/UiColourWheel.hpp>
#include <Uis/UiSection.hpp>
#include <Uis/UiPanel.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test {
class Hierarchy : public UiPanel {
public:
	Hierarchy();

	void UpdateObject() override;

private:
	UiSection section1;
	UiButtonInput button1;

	UiSliderInput sliderR;
	UiSliderInput sliderG;
	UiSliderInput sliderB;
	UiTextInput textHex;
	Gui rgbColour;
	//UiColourWheel colourWheel;
};
}
