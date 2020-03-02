#include "OverlayDebug.hpp"

#include <Scenes/Scenes.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Guis/Gui.hpp>

namespace test {
OverlayDebug::OverlayDebug() {
	/*//slices.SetTransform({{300, 300}, UiAnchor::Centre});
	slices.GetConstraints().SetWidth<PixelConstraint>(300)
		.SetHeight<RatioConstraint>(1.0f)
		.SetX<PixelConstraint>(0, UiAnchor::Centre)
		.SetY<PixelConstraint>(0, UiAnchor::Centre);
	slices.SetImage(Image2d::Create("Guis/9Patch.png"));
	slices.SetNinePatches({0.3333f, 0.3333f, 0.6666f, 0.6666f});
	//slices.GetTransform().SetDepth(-1.0f);
	slices.SetEnabled(false);
	GetParent()->AddChild(&slices);*/

	auto createText = [this](int32_t i, Text &object) {
		//object.SetTransform({{100, 12}, UiAnchor::LeftBottom, {2, -2 - (i * 14)}});
		object.GetConstraints().SetWidth<PixelConstraint>(100)
			.SetHeight<PixelConstraint>(12)
			.SetX<PixelConstraint>(2, UiAnchor::Left)
			.SetY<PixelConstraint>(-2 - (i * 14), UiAnchor::Bottom);
		object.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
		object.SetFontSize(11);
		AddChild(&object);
	};

	createText(0, textFrameTime);
	createText(1, textFps);
	createText(2, textUps);
}

void OverlayDebug::UpdateObject() {
	//slices.GetTransform().SetSize(2.0f * (Mouse::Get()->GetPosition() - (Window::Get()->GetSize() / 2)));

	auto paused = Scenes::Get()->IsPaused();
	textFrameTime.SetTextColour(paused ? Colour::Black : Colour::White);
	textFps.SetTextColour(paused ? Colour::Black : Colour::White);
	textUps.SetTextColour(paused ? Colour::Black : Colour::White);

	textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
