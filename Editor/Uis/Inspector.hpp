#pragma once

#include <Uis/Inputs/UiButtonInput.hpp>
#include <Uis/Inputs/UiGrabberInput.hpp>
#include <Uis/Inputs/UiSliderInput.hpp>
//#include <Uis/Inputs/UiRadioInput.hpp>
#include <Uis/Inputs/UiTextInput.hpp>
#include <Uis/UiSection.hpp>
#include <Uis/UiPanel.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test {
class Inspector : public UiPanel {
public:
	Inspector();

	void UpdateObject() override;

private:
	UiSection section1;
	UiButtonInput button1;
	UiGrabberMouse input1;
	UiGrabberKeyboard input2;
	UiGrabberJoystick input3;
	UiSliderInput slider1;
	UiTextInput text1;
	//UiRadioInput radio1;
	//UiRadioInput radio2;
	//UiRadioInput radio3;
	//UiRadioManager radioManager1;
};
}
