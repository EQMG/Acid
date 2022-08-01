#pragma once

#include <Scenes/Scene.hpp>
#include <Uis/UiStartLogo.hpp>
#include "Uis/OverlayDebug.hpp"

import acid.sound.component;

using namespace acid;

namespace test {
class Scene1 : public Scene {
public:
	Scene1();

	void Start() override;
	void Update() override;
	bool IsPaused() const override;

private:
	Sound soundScreenshot;

	UiStartLogo uiStartLogo;
	OverlayDebug overlayDebug;
};
}
