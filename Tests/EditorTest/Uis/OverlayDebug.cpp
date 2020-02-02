#include "OverlayDebug.hpp"

#include <Uis/Drivers/ConstantDriver.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test {
OverlayDebug::OverlayDebug() {
	m_textFrameTime.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -2}});
	m_textFrameTime.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textFrameTime.SetFontSize(11);
	AddChild(&m_textFrameTime);

	m_textFps.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -16}});
	m_textFps.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textFps.SetFontSize(11);
	AddChild(&m_textFps);

	m_textUps.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -30}});
	m_textUps.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textUps.SetFontSize(11);
	AddChild(&m_textUps);
}

void OverlayDebug::UpdateObject() {
	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
