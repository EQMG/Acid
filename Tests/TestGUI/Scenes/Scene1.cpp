#include "Scene1.hpp"

#include <Inputs/Input.hpp>
#include <Uis/Drivers/DriverConstant.hpp>
#include <Uis/Drivers/DriverSlide.hpp>
#include <Uis/Uis.hpp>

namespace test {
const Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene(std::make_unique<Camera>()),
	m_uiStartLogo(&Uis::Get()->GetCanvas()),
	m_uiPanels(&Uis::Get()->GetCanvas()),
	m_overlayDebug(&Uis::Get()->GetCanvas()) {
	Input::Get()->GetButton("pause")->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			TogglePause();
		}
	}, this);

	//Uis::Get()->GetCanvas().SetScaleDriver<DriverSinwave>(Vector2f(0.3f), Vector2f(5.0f), 16s);

	m_uiStartLogo.SetAlphaDriver<DriverConstant>(1.0f);
	m_overlayDebug.SetAlphaDriver<DriverConstant>(0.0f);
	m_uiPanels.SetAlphaDriver<DriverConstant>(0.0f);

	m_uiStartLogo.OnFinished().Add([this]() {
		m_overlayDebug.SetAlphaDriver<DriverSlide>(0.0f, 1.0f, UI_SLIDE_TIME);
		//m_uiPanels.SetAlphaDriver<DriverSlide>(0.0f, 1.0f, UI_SLIDE_TIME);
		TogglePause();
	});
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

	m_uiPanels.SetAlphaDriver<DriverSlide>(m_uiPanels.GetAlphaDriver()->Get(), IsPaused() ? 0.0f : 1.0f, UI_SLIDE_TIME);
}
}
