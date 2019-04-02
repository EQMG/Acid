#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject(parent, UiBound::Screen),
	m_textFrameTime(this, UiBound(Vector2f(0.002f, 0.998f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
		Colour::White),
	m_textFps(this, UiBound(Vector2f(0.002f, 0.978f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
		Colour::White),
	m_textUps(this, UiBound(Vector2f(0.002f, 0.958f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f, Colour::White)
{
}

void OverlayDebug::UpdateObject()
{
	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
