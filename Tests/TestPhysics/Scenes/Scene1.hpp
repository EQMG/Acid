#pragma once

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

	UiStartLogo m_uiStartLogo;
	OverlayDebug m_overlayDebug;
};
}
