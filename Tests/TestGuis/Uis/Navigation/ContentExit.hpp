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
		Gui *m_background;
		UiInputButton *m_button1;
		UiInputGrabber *m_input1;
		UiInputSlider *m_slider1;
		UiInputText *m_text1;
	public:
		ContentExit(UiObject *parent);

		~ContentExit();

		void UpdateObject() override;

	};
}
