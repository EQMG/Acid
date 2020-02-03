#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>
#include <Guis/Gui.hpp>

using namespace acid;

namespace test {
class OverlayDebug : public UiObject {
public:
	OverlayDebug();

	void UpdateObject() override;

private:
	//Gui m_slices;

	Text m_textFrameTime;
	Text m_textFps;
	Text m_textUps;
};
}
