#include "OverlayDebug.hpp"

#include <Uis/Drivers/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test {
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject(parent, UiTransform(Vector2i(100, 36), UiAnchor::LeftBottom)),
	m_textFrameTime(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -2)), 11, "", FontType::Create("Fonts/ProximaNova-Regular.fnt")),
	m_textFps(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -16)), 11, "", FontType::Create("Fonts/ProximaNova-Regular.fnt")),
	m_textUps(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -30)), 11, "", FontType::Create("Fonts/ProximaNova-Regular.fnt")) {
}

void OverlayDebug::UpdateObject() {
	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
