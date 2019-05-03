#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject(parent, UiBound::Screen),
	//m_slices(this, UiBound(Vector2f(0.5f, 0.5f), UiReference::Centre, UiAspect::Position | UiAspect::Scale), Image2d::Create("Guis/9Patch.png")),
	m_textFrameTime(this, UiBound(Vector2f(0.002f, 0.998f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
		Colour::White),
	m_textFps(this, UiBound(Vector2f(0.002f, 0.978f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
		Colour::White),
	m_textUps(this, UiBound(Vector2f(0.002f, 0.958f), UiReference::BottomLeft), 1.1f, "", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Left, 1.0f,
		Colour::White)
{
	//m_slices.SetNinePatches(Vector4f(0.3333f, 0.3333f, 0.6666f, 0.6666f));
	//m_slices.SetHeight(1.0f);
}

void OverlayDebug::UpdateObject()
{
	//m_slices.GetRectangle().SetSize(2.0f * (Vector2f(0.5f, 0.5f) - Mouse::Get()->GetPosition()));

	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
