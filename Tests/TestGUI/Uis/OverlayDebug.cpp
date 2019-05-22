#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSinwave.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject(parent, UiTransform(Vector2i(100, 36), UiAnchor::LeftBottom)),
	m_slices(parent, UiTransform(Vector2i(300, 300), UiAnchor::Centre), Image2d::Create("Guis/9Patch.png")),
	m_textFrameTime(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -2)), 11, "", 
		FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textFps(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -16)), 11, "", 
		FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textUps(this, UiTransform(Vector2i(100, 12), UiAnchor::LeftBottom, Vector2i(2, -30)), 11, "", 
		FontType::Create("Fonts/ProximaNova", "Regular"))
{
	m_slices.SetNinePatches(Vector4f(0.3333f, 0.3333f, 0.6666f, 0.6666f));
	//m_slices.GetTransform().SetDepth(-1.0f);
	m_slices.SetEnabled(false);
}

void OverlayDebug::UpdateObject()
{
	m_slices.GetTransform().SetSize(2.0f * (Mouse::Get()->GetPosition() - (Window::Get()->GetSize() / 2)));

	auto paused = Scenes::Get()->IsPaused();
	m_textFrameTime.SetTextColour(paused ? Colour::Black : Colour::White);
	m_textFps.SetTextColour(paused ? Colour::Black : Colour::White);
	m_textUps.SetTextColour(paused ? Colour::Black : Colour::White);

	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
