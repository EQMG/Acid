#pragma once

#include <Guis/Gui.hpp>
#include <Uis/Inputs/UiBooleanInput.hpp>
#include <Uis/Inputs/UiButtonInput.hpp>
#include <Uis/Inputs/UiDropdownInput.hpp>
#include <Uis/Inputs/UiGrabberInput.hpp>
#include <Uis/Inputs/UiRadioInput.hpp>
#include <Uis/Inputs/UiSliderInput.hpp>
#include <Uis/Inputs/UiTextInput.hpp>
#include <Uis/UiObject.hpp>
#include <Uis/UiPanel.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RatioConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Drivers/SinewaveDriver.hpp>

using namespace acid;

namespace test {
class Inventory : public UiObject {
public:
	explicit Inventory() {
		SetScaleDriver<SinewaveDriver>(Vector2f(0.9f), Vector2f(1.2f), 6s);
		for (uint32_t i = 0; i < 10; i++) {
			auto colour = Colour::Red.Lerp(Colour::Blue, static_cast<float>(i) / 10.0f);

			auto slot = std::make_unique<Gui>();
			//slot->SetTransform({{48, 48}, UiAnchor::LeftTop, {48 * i, 0}});
			slot->GetConstraints().SetWidth<PixelConstraint>(48)
				.SetHeight<PixelConstraint>(48)
				.SetX<PixelConstraint>(48 * i)
				.SetY<PixelConstraint>(0, UiAnchor::Top);
			slot->SetImage(Image2d::Create("Guis/White.png"));
			slot->SetColourDriver<ConstantDriver>(colour); // TODO: If colour for GUI is like this do the same for text.
			AddChild(slot.get());
			slots.emplace_back(std::move(slot));

			/*auto slotTitle = std::make_unique<Text>(slots[i].get(), UiTransform({24, 16}, UiAnchor::CentreBottom), 12,
				std::to_string(i), FontType::Create("Fonts/ProximaNova-Regular.ttf", "Bold"), Text::Justify::Centre, colour * 0.33f);
			slotTitles.emplace_back(std::move(slotTitle));*/
		}
	}

	void UpdateObject() override {
	}

private:
	std::vector<std::unique_ptr<Gui>> slots;
	//std::vector<std::unique_ptr<Text>> slotTitles;
};

class Panels : public UiObject {
public:
	Panels();

	void UpdateObject() override;

private:
	Gui background;
	Inventory inventory;

	UiPanel gui0;
	UiBooleanInput boolean0;
	UiButtonInput button0;
	UiDropdownInput dropdown0;
	UiGrabberJoystick grabber0;
	UiGrabberKeyboard grabber1;
	UiGrabberMouse grabber2;
	UiRadioInput radio0a;
	UiRadioInput radio0b;
	UiRadioInput radio0c;
	UiRadioManager radioManager0;
	UiSliderInput slider0;
	UiTextInput text0;

	UiTextInput textX;
	UiTextInput textY;
	UiTextInput textZ;

	Gui gui1;
	Gui gui2;
	Text text3;
	Text text1;
};
}
