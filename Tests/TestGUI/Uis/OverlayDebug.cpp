#include "OverlayDebug.hpp"

#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test {
OverlayDebug::OverlayDebug() {
	/*m_slices.SetTransform({{300, 300}, UiAnchor::Centre});
	m_slices.SetImage(Image2d::Create("Guis/9Patch.png"));
	m_slices.SetNinePatches({0.3333f, 0.3333f, 0.6666f, 0.6666f});
	//m_slices.GetTransform().SetDepth(-1.0f);
	m_slices.SetEnabled(false);
	GetParent()->AddChild(&m_slices);*/

	m_textFrameTime.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -2}});
	m_textFrameTime.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textFrameTime.SetFontSize(11);
	this->AddChild(&m_textFrameTime);

	m_textFps.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -16}});
	m_textFps.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textFps.SetFontSize(11);
	this->AddChild(&m_textFps);

	m_textUps.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -30}});
	m_textUps.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textUps.SetFontSize(11);
	this->AddChild(&m_textUps);
}

void OverlayDebug::UpdateObject() {
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
