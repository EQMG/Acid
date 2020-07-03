#include "OverlayDebug.hpp"

#include <Scenes/Scenes.hpp>
#include <Guis/Gui.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include "World/World.hpp"

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
	createText(3, textTime);
}

void OverlayDebug::UpdateObject() {
	// TODO: Update every 0.333 seconds.
	
	textFrameTime.SetString("Frame Time: " + String::To(1000.0f / Engine::Get()->GetFps()) + "ms");
	textFps.SetString("FPS: " + String::To(Engine::Get()->GetFps()));
	textUps.SetString("UPS: " + String::To(Engine::Get()->GetUps()));

	if (auto world = Scenes::Get()->GetScene()->GetSystem<World>()) {
		auto timePercent = (world->GetDayFactor() * 24.0f) + 6.0f;
		auto hour = static_cast<int32_t>(timePercent);
		auto minute = static_cast<int32_t>((timePercent - hour) * 60.0f);

		if (hour > 24) {
			hour -= 24;
		}

		textTime.SetString("Time: " + String::To(hour) + ":" + String::To(minute));
	}
}
}
