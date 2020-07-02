#include "Scene1.hpp"

#include <Inputs/Inputs.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Drivers/SlideDriver.hpp>
#include <Uis/Uis.hpp>

namespace test {
constexpr Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene(std::make_unique<Camera>()) {
	//uiStartLogo.SetTransform({UiMargins::All});
	uiStartLogo.SetAlphaDriver<ConstantDriver>(1.0f);
	uiStartLogo.OnFinished().connect(this, [this]() {
		overlayDebug.SetAlphaDriver<SlideDriver>(0.0f, 1.0f, UI_SLIDE_TIME);
		//uiPanels.SetAlphaDriver<SlideDriver>(0.0f, 1.0f, UI_SLIDE_TIME);
		TogglePause();
	});
	Uis::Get()->GetCanvas().AddChild(&uiStartLogo);

	//uiPanels.SetTransform({UiMargins::All});
	uiPanels.SetAlphaDriver<ConstantDriver>(0.0f);
	Uis::Get()->GetCanvas().AddChild(&uiPanels);

	//overlayDebug.SetTransform({{100, 36}, UiAnchor::LeftBottom});
	overlayDebug.GetConstraints().SetWidth<PixelConstraint>(100)
		.SetHeight<PixelConstraint>(36)
		.SetX<PixelConstraint>(0, UiAnchor::Left)
		.SetY<PixelConstraint>(0, UiAnchor::Bottom);
	overlayDebug.SetAlphaDriver<ConstantDriver>(0.0f);
	Uis::Get()->GetCanvas().AddChild(&overlayDebug);
	
	//Uis::Get()->GetCanvas().SetScaleDriver<SinewaveDriver>(Vector2f(0.3f), Vector2f(5.0f), 16s);
	Inputs::Get()->GetButton("pause")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			TogglePause();
		}
	});
}

void Scene1::Start() {
}

void Scene1::Update() {
}

bool Scene1::IsPaused() const {
	return !uiStartLogo.IsFinished() || uiPanels.GetAlphaDriver()->Get() > 0.0f;
}

void Scene1::TogglePause() {
	if (!uiStartLogo.IsFinished()) {
		return;
	}

	uiPanels.SetAlphaDriver<SlideDriver>(uiPanels.GetAlphaDriver()->Get(), IsPaused() ? 0.0f : 1.0f, UI_SLIDE_TIME);
}
}
