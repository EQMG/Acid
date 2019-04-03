#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputDropdown.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputBoolean.hpp>
#include <Uis/Inputs/UiInputRadio.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputText.hpp>
#include <Uis/UiPanel.hpp>

using namespace acid;

namespace test
{
class ContentExit :
	public UiPanel
{
public:
	explicit ContentExit(UiObject *parent);

	void UpdateObject() override;

private:
	UiInputSlider m_masterVolume;
	UiInputDropdown m_dropdown;
	UiInputSlider m_generalVolume;
	UiInputSlider m_effectVolume;
	UiInputSlider m_musicVolume;
	UiInputButton m_iconify;
	UiInputBoolean m_antialiasing;
	UiInputBoolean m_borderless;
	UiInputBoolean m_resizable;
	UiInputBoolean m_floating;
	UiInputBoolean m_fullscreen;

	UiInputSlider m_fpsLimit;
	UiInputText m_textTitle;

	UiGrabberMouse m_input1;
	UiGrabberKeyboard m_input2;
	UiGrabberJoystick m_input3;
	UiInputRadio m_radio1;
	UiInputRadio m_radio2;
	UiInputRadio m_radio3;
	UiRadioManager m_radioManager1;
};
}