#include "Panels.hpp"

namespace test {
int32_t GetNextY(uint32_t inc = 1) {
	static int32_t i = -1;
	i += inc;
	return i * (UiInputButton::Size.m_y + 6);
}

uint32_t SIZE3_GAP = 2;
auto SIZE3 = (UiInputButton::Size - Vector2f(2 * SIZE3_GAP, 0)) * Vector2f((1.0f / 3.0f), 1.0f);

Panels::Panels() {
	//m_background.SetTransform({UiMargins::All});
	m_background.SetImage(Image2d::Create("Guis/White.png"));
	AddChild(&m_background);

	//m_inventory.SetTransform({{480, 48}, UiAnchor::CentreBottom});
	m_inventory.GetConstraints().SetWidth<PixelConstraint>(480)
		.SetHeight<PixelConstraint>(48)
		.SetX<RelativeConstraint>(0.0f, UiAnchor::Centre)
		.SetY<RelativeConstraint>(0.0f, UiAnchor::Bottom);
	AddChild(&m_inventory);

	//m_gui0.SetTransform({{456, 500}, UiAnchor::LeftTop, {64, 64}});
	m_gui0.SetManipulate(UiManipulate::All);
	m_gui0.SetScrollBars(ScrollBar::Both);
	m_gui0.SetBackgroundColor(UiInputButton::BackgroundColour);
	AddChild(&m_gui0);

	//m_boolean0.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_boolean0.SetTitle("Boolean");
	m_gui0.AddChild(&m_boolean0);

	//m_button0.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_button0.SetTitle("Button");
	m_gui0.AddChild(&m_button0);

	//m_dropdown0.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_dropdown0.SetTitle("Dropdown");
	m_dropdown0.SetOptions({"Option A", "Option B", "Option C"});
	m_dropdown0.SetValue(0);
	m_gui0.AddChild(&m_dropdown0);

	//m_grabber0.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_grabber0.SetTitle("Grabber Joystick");
	m_grabber0.SetPort(0);
	m_grabber0.SetValue(0);
	m_gui0.AddChild(&m_grabber0);

	//m_grabber1.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_grabber1.SetTitle("Grabber Keyboard");
	m_grabber1.SetValue(Key::A);
	m_gui0.AddChild(&m_grabber1);

	//m_grabber2.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_grabber2.SetTitle("Grabber Mouse");
	m_grabber2.SetValue(MouseButton::Left);
	m_gui0.AddChild(&m_grabber2);

	//m_radio0a.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_radio0a.SetTitle("Radio A");
	m_gui0.AddChild(&m_radio0a);

	//m_radio0b.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_radio0b.SetTitle("Radio B");
	m_radio0b.SetValue(true);
	m_gui0.AddChild(&m_radio0b);

	//m_radio0c.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_radio0c.SetTitle("Radio C");
	m_gui0.AddChild(&m_radio0c);

	m_radioManager0.SetMarkType(UiInputRadio::Type::Check);
	m_radioManager0.AddInputs({&m_radio0a, &m_radio0b, &m_radio0c});
	
	//m_slider0.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_slider0.SetTitle("Slider");
	m_slider0.SetValueMin(-100.0f);
	m_slider0.SetValueMax(100.0f);
	m_slider0.SetValue(50.0f);
	m_slider0.SetRoundTo(1);
	m_gui0.AddChild(&m_slider0);

	//m_text0.SetTransform({UiInputButton::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	m_text0.SetTitle("Text");
	m_text0.SetMaxLength(16);
	m_text0.SetValue("ABC123");
	m_gui0.AddChild(&m_text0);

	//m_textX.SetTransform({SIZE3, UiAnchor::LeftTop, {0, GetNextY()}});
	m_textX.SetTitle("X");
	m_textX.SetMaxLength(16);
	m_textX.SetValue("0.0");
	m_gui0.AddChild(&m_textX);

	//m_textY.SetTransform({SIZE3, UiAnchor::LeftTop, {SIZE3.m_x + SIZE3_GAP, GetNextY(0)}});
	m_textY.SetTitle("X");
	m_textY.SetMaxLength(16);
	m_textY.SetValue("1.0");
	m_gui0.AddChild(&m_textY);

	//m_textZ.SetTransform({SIZE3, UiAnchor::LeftTop, {2 * (SIZE3.m_x + SIZE3_GAP), GetNextY(0)}});
	m_textZ.SetTitle("X");
	m_textZ.SetMaxLength(16);
	m_textZ.SetValue("1.0");
	m_gui0.AddChild(&m_textZ);

	//m_gui1.SetTransform({UiMargins::Left | UiMargins::Right | UiMargins::Top, {0.5f, 0.0f}, {0.0f, -100}});
	m_gui1.SetImage(Image2d::Create("Guis/White.png"));
	m_gui1.SetColourDriver<ConstantDriver>(Colour::Blue);
	m_gui1.OnSelected().Add([this](bool selected) {
		m_gui1.SetColourDriver<ConstantDriver>(selected ? Colour::Green : Colour::Blue);
	}, this);
	AddChild(&m_gui1);

	//m_gui2.SetTransform({UiMargins::None, {48, 48}, {-48, -48}});
	m_gui2.SetImage(Image2d::Create("Guis/White.png"));
	m_gui2.SetColourDriver<ConstantDriver>(Colour::Yellow);
	m_gui1.AddChild(&m_gui2);

	//m_text3.SetTransform({UiMargins::None, {24, 24}, {-24, -24}});
	m_text3.SetFontType(FontType::Create("Fonts/LobsterTwo-Italic.ttf"));
	m_text3.SetString("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent hendrerit pharetra dui, vitae blandit purus vehicula blandit. Interdum et malesuada fames ac ante ipsum primis in faucibus. Mauris faucibus nunc vitae fringilla dignissim. Maecenas placerat vulputate cursus. Pellentesque leo sapien, efficitur vehicula tempus pharetra, consequat eu erat. Vestibulum at urna nec mi semper faucibus. Nunc malesuada mi turpis, eu bibendum turpis euismod at. Pellentesque vel nisl bibendum, condimentum urna quis, placerat risus. Sed laoreet blandit felis sit amet lacinia.\n\n"
		"Aenean et tincidunt libero. Integer eu eros tempor, iaculis odio a, imperdiet arcu. Fusce vitae fringilla tellus. Donec blandit nulla mauris. Donec feugiat efficitur ligula eget facilisis. Aenean a nibh efficitur, sodales nisl vitae, ullamcorper lacus. Donec ornare nunc non erat efficitur, sed imperdiet nunc rutrum. Aliquam erat volutpat. Integer feugiat risus non enim facilisis, et luctus erat aliquet.\n\n"
		"Nulla euismod eros massa, sit amet malesuada elit rutrum vel. Sed ullamcorper quam non nulla lobortis, at volutpat nisi lacinia. Nullam ornare a nunc quis facilisis. Sed finibus finibus libero, ut interdum augue. Maecenas id nisl eu arcu eleifend dictum. Sed malesuada ornare nunc vitae pretium. Nulla ut ultricies ipsum. Curabitur vitae odio enim.");
	m_gui2.AddChild(&m_text3);

	//m_text1.SetTransform({{2000, 256}, UiAnchor::LeftBottom, {50, -75}});
	m_text1.SetFontType(FontType::Create("Fonts/ProximaNova-Bold.ttf"));
	m_text1.SetFontSize(72);
	m_text1.SetString("P(t) = (1 - t)² * p₀ + 2t * (1 - t) * p₁ + t² * p₂");
	m_text1.SetEnabled(false);
	AddChild(&m_text1);
}

void Panels::UpdateObject() {
}
}
