#pragma once

#include <Audio/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Inputs/ButtonMouse.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/ButtonCompound.hpp>
#include <Scenes/Scene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"

using namespace acid;

namespace test
{
class Scene1 :
	public Scene
{
public:
	Scene1();

	void Start() override;

	void Update() override;

	bool IsPaused() const override;

private:
	ButtonMouse m_buttonSpawnSphere;
	std::unique_ptr<ButtonCompound> m_buttonCaptureMouse;
	ButtonKeyboard m_buttonSave;
	Sound m_soundScreenshot;

	UiStartLogo m_uiStartLogo;
	OverlayDebug m_overlayDebug;
};
}
