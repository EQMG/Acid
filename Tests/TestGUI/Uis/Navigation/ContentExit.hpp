#pragma once

#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputBoolean.hpp>
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
		UiInputSlider m_generalVolume;
		UiInputSlider m_effectVolume;
		UiInputSlider m_musicVolume;
		UiInputBoolean m_antialiasing;
		UiInputBoolean m_borderless;
		UiInputBoolean m_resizable;
		UiInputBoolean m_floating;
		UiInputBoolean m_fullscreen;
		UiInputSlider m_fpsLimit;
	};
}