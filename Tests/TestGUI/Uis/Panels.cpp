#include "Panels.hpp"

namespace test {
int32_t GetNextY(uint32_t inc = 1) {
	static int32_t i = -1;
	i += inc;
	return i * (UiInputButton::Size.m_y + 6);
}

uint32_t SIZE3_GAP = 2;
auto SIZE3 = (UiInputButton::Size - Vector2f(2 * SIZE3_GAP, 0)) * Vector2f((1.0f / 3.0f), 1.0f);

Panels::Panels(UiObject *parent) :
	UiObject(parent, {UiMargins::All}),
	m_background(this, {UiMargins::All}, Image2d::Create("Guis/White.png")),
	m_inventory(this),
	m_gui0(this, {{456, 500}, UiAnchor::LeftTop, {64, 64}}, UiInputButton::BackgroundColour, UiManipulate::All, ScrollBar::Both),
	m_boolean0(&m_gui0.GetContent(), "Boolean", false, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_button0(&m_gui0.GetContent(), "Button", {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_dropdown0(&m_gui0.GetContent(), "Dropdown", 0, {"Option A", "Option B", "Option C"}, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_grabber0(&m_gui0.GetContent(), "Grabber Joystick", 0, 0, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_grabber1(&m_gui0.GetContent(), "Grabber Keyboard", Key::A, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_grabber2(&m_gui0.GetContent(), "Grabber Mouse", MouseButton::Left, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_radio0a(&m_gui0.GetContent(), "Radio A", UiInputRadio::Type::Filled, false, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_radio0b(&m_gui0.GetContent(), "Radio B", UiInputRadio::Type::Filled, true, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_radio0c(&m_gui0.GetContent(), "Radio C", UiInputRadio::Type::Filled, false, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_radioManager0(UiInputRadio::Type::Check, false, {&m_radio0a, &m_radio0b, &m_radio0c}),
	m_slider0(&m_gui0.GetContent(), "Slider", 50.0f, -100.0f, 100.0f, 1, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_text0(&m_gui0.GetContent(), "Text", "ABC123", 16, {UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_textX(&m_gui0.GetContent(), "X", "0.0", 16, {SIZE3, UiAnchor::LeftTop, {0, GetNextY()}}),
	m_textY(&m_gui0.GetContent(), "Y", "1.0", 16, {SIZE3, UiAnchor::LeftTop, {SIZE3.m_x + SIZE3_GAP, GetNextY(0)}}),
	m_textZ(&m_gui0.GetContent(), "Z", "1.0", 16, {SIZE3, UiAnchor::LeftTop, {2 * (SIZE3.m_x + SIZE3_GAP), GetNextY(0)}}),
	m_gui1(this, {UiMargins::Left | UiMargins::Right | UiMargins::Top, {0.5f, 0.0f}, {0.0f, -100}}, Image2d::Create("Guis/White.png")),
	m_gui2(&m_gui1, {UiMargins::None, {48, 48}, {-48, -48}}, Image2d::Create("Guis/White.png")),
	m_text3(&m_gui2, {UiMargins::None, {24, 24}, {-24, -24}}, 12, "", FontType::Create("Fonts/ProximaNova-Regular.ttf")),
	m_text1(this, {{256, 256}, UiAnchor::LeftBottom, {50, -75}}, 72, "|ABC abc 0123_*&.", FontType::Create("Fonts/ProximaNova-Regular.ttf")) {
	m_gui1.SetColourDriver(std::make_unique<DriverConstant<Colour>>(Colour::Blue));
	m_gui2.SetColourDriver(std::make_unique<DriverConstant<Colour>>(Colour::Yellow));
	m_gui1.OnSelected().Add([this](bool selected) {
		if (selected) {
			m_gui1.SetColourDriver(std::make_unique<DriverConstant<Colour>>(Colour::Green));
		} else {
			m_gui1.SetColourDriver(std::make_unique<DriverConstant<Colour>>(Colour::Blue));
		}
	});
	m_text3.SetString("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent hendrerit pharetra dui, vitae blandit purus vehicula blandit. Interdum et malesuada fames ac ante ipsum primis in faucibus. Mauris faucibus nunc vitae fringilla dignissim. Maecenas placerat vulputate cursus. Pellentesque leo sapien, efficitur vehicula tempus pharetra, consequat eu erat. Vestibulum at urna nec mi semper faucibus. Nunc malesuada mi turpis, eu bibendum turpis euismod at. Pellentesque vel nisl bibendum, condimentum urna quis, placerat risus. Sed laoreet blandit felis sit amet lacinia.\n\n"
		"Aenean et tincidunt libero. Integer eu eros tempor, iaculis odio a, imperdiet arcu. Fusce vitae fringilla tellus. Donec blandit nulla mauris. Donec feugiat efficitur ligula eget facilisis. Aenean a nibh efficitur, sodales nisl vitae, ullamcorper lacus. Donec ornare nunc non erat efficitur, sed imperdiet nunc rutrum. Aliquam erat volutpat. Integer feugiat risus non enim facilisis, et luctus erat aliquet.\n\n"
		"Nulla euismod eros massa, sit amet malesuada elit rutrum vel. Sed ullamcorper quam non nulla lobortis, at volutpat nisi lacinia. Nullam ornare a nunc quis facilisis. Sed finibus finibus libero, ut interdum augue. Maecenas id nisl eu arcu eleifend dictum. Sed malesuada ornare nunc vitae pretium. Nulla ut ultricies ipsum. Curabitur vitae odio enim.");
	m_text1.SetEnabled(false);
}

void Panels::UpdateObject() {
}
}
