#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputText.hpp>
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
	UiInputButton button1;

	UiInputSlider sliderR;
	UiInputSlider sliderG;
	UiInputSlider sliderB;
	UiInputText textHex;
	Gui rgbColour;
	//UiColourWheel colourWheel;
};
}
