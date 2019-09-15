#pragma once

#include <Uis/UiObject.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test {
class OverlayDebug : public UiObject {
public:
	explicit OverlayDebug(UiObject *parent);

	void UpdateObject() override;

private:
	Text m_textFrameTime;
	Text m_textFps;
	Text m_textUps;
	Text m_textTime;
};
}
