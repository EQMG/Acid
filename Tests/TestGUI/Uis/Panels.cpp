#include "Panels.hpp"

namespace test {
int32_t GetNextY(uint32_t inc = 1) {
	static int32_t i = -1;
	i += inc;
	return i * (UiButtonInput::Size.y + 6);
}

constexpr uint32_t SIZE3_GAP = 2;
constexpr auto SIZE3 = (UiButtonInput::Size - Vector2f(2 * SIZE3_GAP, 0)) * Vector2f((1.0f / 3.0f), 1.0f);

Panels::Panels() {
	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/White.png"));
	AddChild(&background);

	//inventory.SetTransform({{480, 48}, UiAnchor::CentreBottom});
	inventory.GetConstraints().SetWidth<PixelConstraint>(480)
		.SetHeight<PixelConstraint>(48)
		.SetX<RelativeConstraint>(0.0f, UiAnchor::Centre)
		.SetY<RelativeConstraint>(0.0f, UiAnchor::Bottom);
	AddChild(&inventory);

	//gui0.SetTransform({{456, 500}, UiAnchor::LeftTop, {64, 64}});
	gui0.SetManipulate(UiManipulate::All);
	gui0.SetScrollBars(ScrollBar::Both);
	gui0.SetBackgroundColor(UiButtonInput::BackgroundColour);
	AddChild(&gui0);

	//boolean0.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	boolean0.SetTitle("Boolean");
	gui0.AddChild(&boolean0);

	//button0.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	button0.SetTitle("Button");
	gui0.AddChild(&button0);

	//dropdown0.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	dropdown0.SetTitle("Dropdown");
	dropdown0.SetOptions({"Option A", "Option B", "Option C"});
	dropdown0.SetValue(0);
	gui0.AddChild(&dropdown0);

	//grabber0.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	grabber0.SetTitle("Grabber Joystick");
	grabber0.SetPort(JoystickPort::_1);
	grabber0.SetValue(0);
	gui0.AddChild(&grabber0);

	//grabber1.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	grabber1.SetTitle("Grabber Keyboard");
	grabber1.SetValue(Key::A);
	gui0.AddChild(&grabber1);

	//grabber2.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	grabber2.SetTitle("Grabber Mouse");
	grabber2.SetValue(MouseButton::Left);
	gui0.AddChild(&grabber2);

	//radio0a.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	radio0a.SetTitle("Radio A");
	gui0.AddChild(&radio0a);

	//radio0b.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	radio0b.SetTitle("Radio B");
	radio0b.SetValue(true);
	gui0.AddChild(&radio0b);

	//radio0c.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	radio0c.SetTitle("Radio C");
	gui0.AddChild(&radio0c);

	radioManager0.SetMarkType(UiRadioInput::Type::Check);
	radioManager0.AddInputs({&radio0a, &radio0b, &radio0c});
	
	//slider0.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	slider0.SetTitle("Slider");
	slider0.SetValueMin(-100.0f);
	slider0.SetValueMax(100.0f);
	slider0.SetValue(50.0f);
	slider0.SetRoundTo(1);
	gui0.AddChild(&slider0);

	//text0.SetTransform({UiButtonInput::Size, UiAnchor::LeftTop, {0, GetNextY()}});
	text0.SetTitle("Text");
	text0.SetMaxLength(16);
	text0.SetValue("ABC123");
	gui0.AddChild(&text0);

	//textX.SetTransform({SIZE3, UiAnchor::LeftTop, {0, GetNextY()}});
	textX.SetTitle("X");
	textX.SetMaxLength(16);
	textX.SetValue("0.0");
	gui0.AddChild(&textX);

	//textY.SetTransform({SIZE3, UiAnchor::LeftTop, {SIZE3.x + SIZE3_GAP, GetNextY(0)}});
	textY.SetTitle("X");
	textY.SetMaxLength(16);
	textY.SetValue("1.0");
	gui0.AddChild(&textY);

	//textZ.SetTransform({SIZE3, UiAnchor::LeftTop, {2 * (SIZE3.x + SIZE3_GAP), GetNextY(0)}});
	textZ.SetTitle("X");
	textZ.SetMaxLength(16);
	textZ.SetValue("1.0");
	gui0.AddChild(&textZ);

	//gui1.SetTransform({UiMargins::Left | UiMargins::Right | UiMargins::Top, {0.5f, 0.0f}, {0.0f, -100}});
	gui1.SetImage(Image2d::Create("Guis/White.png"));
	gui1.SetColourDriver<ConstantDriver>(Colour::Blue);
	gui1.OnSelected().connect(this, [this](bool selected) {
		gui1.SetColourDriver<ConstantDriver>(selected ? Colour::Green : Colour::Blue);
	});
	AddChild(&gui1);

	//gui2.SetTransform({UiMargins::None, {48, 48}, {-48, -48}});
	gui2.SetImage(Image2d::Create("Guis/White.png"));
	gui2.SetColourDriver<ConstantDriver>(Colour::Yellow);
	gui1.AddChild(&gui2);

	//text3.SetTransform({UiMargins::None, {24, 24}, {-24, -24}});
	text3.SetFontType(FontType::Create("Fonts/LobsterTwo-Italic.ttf"));
	text3.SetString("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent hendrerit pharetra dui, vitae blandit purus vehicula blandit. Interdum et malesuada fames ac ante ipsum primis in faucibus. Mauris faucibus nunc vitae fringilla dignissim. Maecenas placerat vulputate cursus. Pellentesque leo sapien, efficitur vehicula tempus pharetra, consequat eu erat. Vestibulum at urna nec mi semper faucibus. Nunc malesuada mi turpis, eu bibendum turpis euismod at. Pellentesque vel nisl bibendum, condimentum urna quis, placerat risus. Sed laoreet blandit felis sit amet lacinia.\n\n"
		"Aenean et tincidunt libero. Integer eu eros tempor, iaculis odio a, imperdiet arcu. Fusce vitae fringilla tellus. Donec blandit nulla mauris. Donec feugiat efficitur ligula eget facilisis. Aenean a nibh efficitur, sodales nisl vitae, ullamcorper lacus. Donec ornare nunc non erat efficitur, sed imperdiet nunc rutrum. Aliquam erat volutpat. Integer feugiat risus non enim facilisis, et luctus erat aliquet.\n\n"
		"Nulla euismod eros massa, sit amet malesuada elit rutrum vel. Sed ullamcorper quam non nulla lobortis, at volutpat nisi lacinia. Nullam ornare a nunc quis facilisis. Sed finibus finibus libero, ut interdum augue. Maecenas id nisl eu arcu eleifend dictum. Sed malesuada ornare nunc vitae pretium. Nulla ut ultricies ipsum. Curabitur vitae odio enim.");
	gui2.AddChild(&text3);

	//text1.SetTransform({{2000, 256}, UiAnchor::LeftBottom, {50, -75}});
	text1.SetFontType(FontType::Create("Fonts/ProximaNova-Bold.ttf"));
	text1.SetFontSize(72);
	text1.SetString("P(t) = (1 - t)² * p₀ + 2t * (1 - t) * p₁ + t² * p₂");
	text1.SetEnabled(false);
	AddChild(&text1);
}

void Panels::UpdateObject() {
}
}
