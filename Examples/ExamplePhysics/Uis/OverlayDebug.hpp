#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test {
class OverlayDebug : public UiObject {
public:
	OverlayDebug();

	void UpdateObject() override;

private:
	Text textFrameTime;
	Text textFps;
	Text textUps;
	Text textTime;
};
}
