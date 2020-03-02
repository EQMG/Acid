#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
//#include <Uis/Inputs/UiInputRadio.hpp>
#include <Uis/Inputs/UiInputText.hpp>
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
	UiInputButton button1;
	UiGrabberMouse input1;
	UiGrabberKeyboard input2;
	UiGrabberJoystick input3;
	UiInputSlider slider1;
	UiInputText text1;
	//UiInputRadio radio1;
	//UiInputRadio radio2;
	//UiInputRadio radio3;
	//UiRadioManager radioManager1;
};
}
