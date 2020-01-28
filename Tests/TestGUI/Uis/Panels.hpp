#pragma once

#include <Guis/Gui.hpp>
#include <Uis/Inputs/UiInputBoolean.hpp>
#include <Uis/Inputs/UiInputButton.hpp>
#include <Uis/Inputs/UiInputDropdown.hpp>
#include <Uis/Inputs/UiInputGrabber.hpp>
#include <Uis/Inputs/UiInputRadio.hpp>
#include <Uis/Inputs/UiInputSlider.hpp>
#include <Uis/Inputs/UiInputText.hpp>
#include <Uis/UiObject.hpp>
#include <Uis/UiPanel.hpp>
#include <Uis/Drivers/DriverConstant.hpp>
#include <Uis/Drivers/DriverSinwave.hpp>

using namespace acid;

namespace test {
class Inventory : public UiObject {
public:
	explicit Inventory(UiObject *parent) :
		UiObject(parent, {{480, 48}, UiAnchor::CentreBottom}) {
		SetScaleDriver(std::make_unique<DriverSinwave<Vector2f>>(Vector2f(0.9f), Vector2f(1.2f), 6s));
		for (uint32_t i = 0; i < 10; i++) {
			auto colour = Colour::Red.Lerp(Colour::Blue, static_cast<float>(i) / 10.0f);

			auto slot = std::make_unique<Gui>(this, UiTransform({48, 48}, UiAnchor::LeftTop, {48 * i, 0}), Image2d::Create("Guis/White.png"));
			slot->SetColourDriver(std::make_unique<DriverConstant<Colour>>(colour)); // TODO: If colour for GUI is like this do the same for text.
			m_slots.emplace_back(std::move(slot));

			/*auto slotTitle = std::make_unique<Text>(m_slots[i].get(), UiTransform({24, 16}, UiAnchor::CentreBottom), 12,
				std::to_string(i), FontType::Create("Fonts/ProximaNova-Regular.fnt", "Bold"), Text::Justify::Centre, colour * 0.33f);
			m_slotTitles.emplace_back(std::move(slotTitle));*/
		}
	}

	void UpdateObject() override {
	}

private:
	std::vector<std::unique_ptr<Gui>> m_slots;
	//std::vector<std::unique_ptr<Text>> m_slotTitles;
};

class Panels : public UiObject {
public:
	explicit Panels(UiObject *parent);

	void UpdateObject() override;

private:
	Gui m_background;
	Inventory m_inventory;

	UiPanel m_gui0;
	UiInputBoolean m_boolean0;
	UiInputButton m_button0;
	UiInputDropdown m_dropdown0;
	UiGrabberJoystick m_grabber0;
	UiGrabberKeyboard m_grabber1;
	UiGrabberMouse m_grabber2;
	UiInputRadio m_radio0a;
	UiInputRadio m_radio0b;
	UiInputRadio m_radio0c;
	UiRadioManager m_radioManager0;
	UiInputSlider m_slider0;
	UiInputText m_text0;

	UiInputText m_textX;
	UiInputText m_textY;
	UiInputText m_textZ;

	Gui m_gui1;
	Gui m_gui2;
	Text m_text3;
	Text m_text1;
};
}
