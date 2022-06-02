#pragma once

#include <Scenes/Scene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/Panels.hpp"
#include "Uis/OverlayDebug.hpp"

using namespace acid;

namespace test {
class Scene1 : public Scene {
public:
	Scene1();

	void Start() override;
	void Update() override;
	bool IsPaused() const override;

	void TogglePause();

private:
	UiStartLogo uiStartLogo;
	Panels uiPanels;
	OverlayDebug overlayDebug;
};
}
