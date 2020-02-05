#include "Inspector.hpp"

#include <Audio/Audio.hpp>
#include <Devices/Keyboard.hpp>

namespace test {
Inspector::Inspector() {
	//m_section1.SetTransform({{0.08f, 0.05f}, UiAnchor::LeftTop});
	m_section1.SetTitle("Section Right");
	UiPanel::AddChild(&m_section1);

	//m_button1.SetTransform({{0.0f, 0.0f}, UiAnchor::LeftTop});
	m_button1.SetTitle("Button #2");
	m_section1.AddChild(&m_button1);

	//m_input1.SetTransform({{0.0f, 0.06f}, UiAnchor::LeftTop});
	m_input1.SetTitle("Mouse Grabber");
	m_input1.SetValue(MouseButton::Left);
	m_section1.AddChild(&m_input1);

	//m_input2.SetTransform({{0.0f, 0.12f}, UiAnchor::LeftTop});
	m_input2.SetTitle("Keyboard Grabber");
	m_input2.SetValue(Key::W);
	m_section1.AddChild(&m_input2);

	//m_input3.SetTransform({{0.0f, 0.18f}, UiAnchor::LeftTop});
	m_input3.SetTitle("Joystick Grabber");
	m_input3.SetPort(0);
	m_input3.SetValue(3);
	m_section1.AddChild(&m_input3);

	//m_slider1.SetTransform({{0.0f, 0.24f}, UiAnchor::LeftTop});
	m_slider1.SetTitle("Volume");
	m_slider1.SetValueMin(0.0f);
	m_slider1.SetValueMax(100.0f);
	m_slider1.SetRoundTo(0);
	m_slider1.SetValue(100.0f);
	m_slider1.OnValue().Add([this](float value) {
		Audio::Get()->SetGain(Audio::Type::Effect, value);
	}, this);
	m_section1.AddChild(&m_slider1);

	//m_text1.SetTransform({{0.0f, 0.30f}, UiAnchor::LeftTop});
	m_text1.SetTitle("Title");
	m_text1.SetMaxLength(14);
	m_text1.SetValue(Window::Get()->GetTitle());
	m_text1.OnValue().Add([this](std::string text) {
		Window::Get()->SetTitle(text);
	}, this);
	m_section1.AddChild(&m_text1);

	//m_radio1.SetTransform({{0.0f, 0.36f}, UiAnchor::LeftTop});
	//m_radio1.SetTitle("First Value");
	//m_section1.SetValue(&m_radio1);

	//m_radio2.SetTransform({{0.0f, 0.405f}, UiAnchor::LeftTop});
	//m_radio2.SetTitle("Second Value");
	//m_radio2.SetValue(true);
	//m_section1.SetValue(&m_radio2);

	//m_radio3.SetTransform({{0.0f, 0.45f}, UiAnchor::LeftTop});
	//m_radio3.SetTitle("Third Value");
	//m_section1.SetValue(&m_radio3);

	//m_radioManager1.SetType(UiInputRadio::Type::Filled);
	//m_radioManager1.SetInputs({&m_radio1, &m_radio2, &m_radio3});

	SetManipulate(UiManipulate::None);
	SetScrollBars(ScrollBar::None);
	SetBackgroundColor(UiInputButton::BackgroundColour);
}

void Inspector::UpdateObject() {
	UiPanel::UpdateObject();
}
}
