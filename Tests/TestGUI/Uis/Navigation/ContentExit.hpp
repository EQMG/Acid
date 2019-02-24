#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputBoolean.hpp>
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
	public:
		explicit ContentExit(UiObject *parent);

		void UpdateObject() override;
	private:
		UiInputSlider m_masterVolume;
		UiInputSlider m_generalVolume;
		UiInputSlider m_effectVolume;
		UiInputSlider m_musicVolume;
		UiInputBoolean m_antialiasing;

		UiInputSlider m_fpsLimit;
	};
}