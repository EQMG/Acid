#include "OverlayDebug.hpp"

#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>

namespace test {
OverlayDebug::OverlayDebug() {
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
	textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));
}
}
