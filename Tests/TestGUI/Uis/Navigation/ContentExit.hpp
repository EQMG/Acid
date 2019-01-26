#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputText.hpp>
#include <Uis/UiObject.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test
{
	class ContentExit :
		public UiObject
	{
	private:
		std::unique_ptr<Gui> m_background;
		std::unique_ptr<UiInputButton> m_button1;
		std::unique_ptr<UiInputButton> m_button2;
		std::unique_ptr<UiGrabberMouse> m_input1;
		std::unique_ptr<UiGrabberKeyboard> m_input2;
		std::unique_ptr<UiGrabberJoystick> m_input3;
		std::unique_ptr<UiInputSlider> m_slider1;
		std::unique_ptr<UiInputText> m_text1;
	public:
		explicit ContentExit(UiObject *parent);

		void UpdateObject() override;
	};
}
