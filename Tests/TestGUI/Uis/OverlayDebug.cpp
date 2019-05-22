#include "OverlayDebug.hpp"

#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSinwave.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test
{
OverlayDebug::OverlayDebug(UiObject *parent) :
	UiObject(parent, UiTransform(Vector2i(100, 36), Vector2i(), UiAnchor::LeftBottom)),
	m_slices(parent, UiTransform(Vector2i(300, 300), Vector2i(), UiAnchor::Centre), Image2d::Create("Guis/9Patch.png")),
	m_textFrameTime(this, UiTransform(Vector2i(100, 12), Vector2i(2, -2), UiAnchor::LeftBottom), 11, "", FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textFps(this, UiTransform(Vector2i(100, 12), Vector2i(2, -16), UiAnchor::LeftBottom), 11, "", FontType::Create("Fonts/ProximaNova", "Regular")),
	m_textUps(this, UiTransform(Vector2i(100, 12), Vector2i(2, -30), UiAnchor::LeftBottom), 11, "", FontType::Create("Fonts/ProximaNova", "Regular"))
{
	//SetScaleDriver(new DriverSinwave<Vector2f>(Vector2f(0.5f), Vector2f(1.5f), 4s));
	m_slices.SetNinePatches(Vector4f(0.3333f, 0.3333f, 0.6666f, 0.6666f));
	//m_slices.SetNinePatches(Vector4i(100, 100, 200, 200));
	//m_slices.GetTransform().SetDepth(-1.0f);
	m_slices.SetEnabled(false);
}

void OverlayDebug::UpdateObject()
{
	m_slices.GetTransform().SetSize(2.0f * (Mouse::Get()->GetPosition() - (Window::Get()->GetSize() / 2)));

	m_textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	m_textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	m_textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
