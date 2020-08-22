#include "Inspector.hpp"

#include <Audio/Audio.hpp>
#include <Devices/Keyboard.hpp>

namespace test {
Inspector::Inspector() {
	//section1.SetTransform({{0.08f, 0.05f}, UiAnchor::LeftTop});
	section1.SetTitle("Section Right");
	UiPanel::AddChild(&section1);

	//button1.SetTransform({{0.0f, 0.0f}, UiAnchor::LeftTop});
	button1.SetTitle("Button #2");
	section1.AddChild(&button1);

	//input1.SetTransform({{0.0f, 0.06f}, UiAnchor::LeftTop});
	input1.SetTitle("Mouse Grabber");
	input1.SetValue(MouseButton::Left);
	section1.AddChild(&input1);

	//input2.SetTransform({{0.0f, 0.12f}, UiAnchor::LeftTop});
	input2.SetTitle("Keyboard Grabber");
	input2.SetValue(Key::W);
	section1.AddChild(&input2);

	//input3.SetTransform({{0.0f, 0.18f}, UiAnchor::LeftTop});
	input3.SetTitle("Joystick Grabber");
	input3.SetPort(JoystickPort::_1);
	input3.SetValue(3);
	section1.AddChild(&input3);

	//slider1.SetTransform({{0.0f, 0.24f}, UiAnchor::LeftTop});
	slider1.SetTitle("Volume");
	slider1.SetValueMin(0.0f);
	slider1.SetValueMax(100.0f);
	slider1.SetRoundTo(0);
	slider1.SetValue(100.0f);
	slider1.OnValue().connect(this, [this](float value) {
		Audio::Get()->SetGain(Audio::Type::Effect, value);
	});
	section1.AddChild(&slider1);

	//text1.SetTransform({{0.0f, 0.30f}, UiAnchor::LeftTop});
	text1.SetTitle("Title");
	text1.SetMaxLength(14);
	text1.SetValue(Window::Get()->GetTitle());
	text1.OnValue().connect(this, [this](std::string text) {
		Window::Get()->SetTitle(text);
	});
	section1.AddChild(&text1);

	//radio1.SetTransform({{0.0f, 0.36f}, UiAnchor::LeftTop});
	//radio1.SetTitle("First Value");
	//section1.SetValue(&radio1);

	//radio2.SetTransform({{0.0f, 0.405f}, UiAnchor::LeftTop});
	//radio2.SetTitle("Second Value");
	//radio2.SetValue(true);
	//section1.SetValue(&radio2);

	//radio3.SetTransform({{0.0f, 0.45f}, UiAnchor::LeftTop});
	//radio3.SetTitle("Third Value");
	//section1.SetValue(&radio3);

	//radioManager1.SetType(UiRadioInput::Type::Filled);
	//radioManager1.SetInputs({&radio1, &radio2, &radio3});

	SetManipulate(UiManipulate::None);
	SetScrollBars(ScrollBar::None);
	SetBackgroundColor(UiButtonInput::BackgroundColour);
}

void Inspector::UpdateObject() {
	UiPanel::UpdateObject();
}
}
