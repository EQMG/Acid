#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>
#include <Uis/Inputs/UiBooleanInput.hpp>
#include <Uis/Inputs/UiSliderInput.hpp>
#include <Uis/UiPanel.hpp>

using namespace acid;

namespace test {
class Pannable : public UiObject {
public:
	Pannable();

	void UpdateObject() override;

private:
	float zoom = 1.0f;

	UiObject content;
	Text title;
	Text body;

	UiPanel settings;
	UiSliderInput masterVolume;
	UiBooleanInput antialiasing;

	Text textFrameTime;
	Text textFps;
	Text textUps;
};
}
