#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputRadio.hpp>
#include <Uis/Inputs/UiInputText.hpp>
#include <Uis/Inputs/UiColourWheel.hpp>
#include <Uis/UiPanel.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class ContentExit :
		public UiPanel
	{
	private:
		std::unique_ptr<UiInputButton> m_button1;
		std::unique_ptr<UiInputButton> m_button2;
		std::unique_ptr<UiInputButton> m_button3;
		std::unique_ptr<UiInputRadio> m_radio1;
		std::unique_ptr<UiInputRadio> m_radio2;
		std::unique_ptr<UiInputRadio> m_radio3;
		UiRadioManager m_radioManager;
		std::unique_ptr<Gui> m_image1;
		std::unique_ptr<UiGrabberMouse> m_input1;
		std::unique_ptr<UiGrabberKeyboard> m_input2;
		std::unique_ptr<UiGrabberJoystick> m_input3;
		std::unique_ptr<UiInputSlider> m_slider1;
		std::unique_ptr<UiInputText> m_text1;

		std::unique_ptr<UiInputSlider> m_sliderR;
		std::unique_ptr<UiInputSlider> m_sliderG;
		std::unique_ptr<UiInputSlider> m_sliderB;
		std::unique_ptr<UiColourWheel> m_colourWheel;
		std::unique_ptr<Gui> m_rgbColour;
	public:
		explicit ContentExit(UiObject *parent);

		void UpdateObject() override;
	};
}
