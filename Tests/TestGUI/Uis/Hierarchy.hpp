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
	UiSection m_section1;
	UiInputButton m_button1;

	UiInputSlider m_sliderR;
	UiInputSlider m_sliderG;
	UiInputSlider m_sliderB;
	UiInputText m_textHex;
	Gui m_rgbColour;
	UiColourWheel m_colourWheel;
};
}
