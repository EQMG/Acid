#pragma once

#include <Uis/UiObject.hpp>
#include <Uis/UiInputButton.hpp>
#include <Uis/UiInputGrabber.hpp>
#include <Uis/UiInputSlider.hpp>
#include <Uis/UiInputText.hpp>
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
		std::unique_ptr<UiInputGrabber> m_input1;
		std::unique_ptr<UiInputSlider> m_slider1;
		std::unique_ptr<UiInputText> m_text1;
	public:
		explicit ContentExit(UiObject *parent);

		void UpdateObject() override;
	};
}
