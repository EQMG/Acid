#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputRadio.hpp>
#include <Uis/Inputs/UiInputText.hpp>
#include <Uis/Inputs/UiColourWheel.hpp>
#include <Uis/UiSection.hpp>
#include <Uis/UiPanel.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class Inspector :
		public UiPanel
	{
	public:
		explicit Inspector(UiObject *parent);

		void UpdateObject() override;

	private:
		UiSection m_section1;
		UiInputButton m_button1;
		UiGrabberMouse m_input1;
		UiGrabberKeyboard m_input2;
		UiGrabberJoystick m_input3;
		UiInputSlider m_slider1;
		UiInputText m_text1;
		UiInputRadio m_radio1;
		UiInputRadio m_radio2;
		UiInputRadio m_radio3;
		UiRadioManager m_radioManager1;
	};
}
