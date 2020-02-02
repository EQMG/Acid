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
	float m_zoom = 1.0f;

	UiObject m_content;
	Text m_title;
	Text m_body;

	UiPanel m_settings;
	UiInputSlider m_masterVolume;
	UiInputBoolean m_antialiasing;

	Text m_textFrameTime;
	Text m_textFps;
	Text m_textUps;
};
}
