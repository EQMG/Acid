#include "Scene1.hpp"

#include <Inputs/Input.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Drivers/SlideDriver.hpp>
#include <Uis/Uis.hpp>

namespace test {
const Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene(std::make_unique<Camera>()) {
	m_uiStartLogo.SetTransform({UiMargins::All});
	m_uiStartLogo.SetAlphaDriver<ConstantDriver>(1.0f);
	m_uiStartLogo.OnFinished().Add([this]() {
		m_overlayDebug.SetAlphaDriver<SlideDriver>(0.0f, 1.0f, UI_SLIDE_TIME);
		//m_uiPanels.SetAlphaDriver<SlideDriver>(0.0f, 1.0f, UI_SLIDE_TIME);
		TogglePause();
	});
	Uis::Get()->GetCanvas().AddChild(&m_uiStartLogo);

	m_uiPanels.SetTransform({UiMargins::All});
	m_uiPanels.SetAlphaDriver<ConstantDriver>(0.0f);
	Uis::Get()->GetCanvas().AddChild(&m_uiPanels);

	m_overlayDebug.SetTransform({{100, 36}, UiAnchor::LeftBottom});
	m_overlayDebug.SetAlphaDriver<ConstantDriver>(0.0f);
	Uis::Get()->GetCanvas().AddChild(&m_overlayDebug);
	
	//Uis::Get()->GetCanvas().SetScaleDriver<SinewaveDriver>(Vector2f(0.3f), Vector2f(5.0f), 16s);
	Input::Get()->GetButton("pause")->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			TogglePause();
		}
	}, this);
}

void Scene1::Start() {
}

void Scene1::Update() {
}

bool Scene1::IsPaused() const {
	return !m_uiStartLogo.IsFinished() || m_uiPanels.GetAlphaDriver()->Get() > 0.0f;
}

void Scene1::TogglePause() {
	if (!m_uiStartLogo.IsFinished()) {
		return;
	}

	m_uiPanels.SetAlphaDriver<SlideDriver>(m_uiPanels.GetAlphaDriver()->Get(), IsPaused() ? 0.0f : 1.0f, UI_SLIDE_TIME);
}
}
