#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>
#include <Uis/Inputs/UiInputBoolean.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
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
	UiInputSlider masterVolume;
	UiInputBoolean antialiasing;

	Text textFrameTime;
	Text textFps;
	Text textUps;
};
}
