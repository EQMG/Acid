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
	class Hierarchy :
		public UiPanel
	{
	public:
		explicit Hierarchy(UiObject *parent);

		void UpdateObject() override;

	private:
		std::unique_ptr<UiSection> m_section1;
		std::unique_ptr<UiInputButton> m_button1;

		std::unique_ptr<UiInputSlider> m_sliderR;
		std::unique_ptr<UiInputSlider> m_sliderG;
		std::unique_ptr<UiInputSlider> m_sliderB;
		std::unique_ptr<UiInputText> m_textHex;
		std::unique_ptr<Gui> m_rgbColour;
		std::unique_ptr<UiColourWheel> m_colourWheel;
	};
}
