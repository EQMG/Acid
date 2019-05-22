#include "Inspector.hpp"

#include <Audio/Audio.hpp>
#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
Inspector::Inspector(UiObject *parent) :
	UiPanel(parent, UiTransform(Vector2f(1.0f, 0.0f), UiAnchor::RightTop, Vector2f(0.3f, 1.0f)), UiInputButton::BackgroundColour, Resize::Right,
		ScrollBar::None),
	m_section1(&GetContent(), "Section Right", UiTransform(Vector2f(0.08f, 0.05f), UiAnchor::LeftTop)),
	m_button1(&m_section1.GetContent(), "Button #2", UiTransform(Vector2f(0.0f, 0.0f), UiAnchor::LeftTop)),
	m_input1(&m_section1.GetContent(), "Mouse Grabber", MouseButton::Left, UiTransform(Vector2f(0.0f, 0.06f), UiAnchor::LeftTop)),
	m_input2(&m_section1.GetContent(), "Keyboard Grabber", Key::W, UiTransform(Vector2f(0.0f, 0.12f), UiAnchor::LeftTop)),
	m_input3(&m_section1.GetContent(), "Joystick Grabber", 0, 3, UiTransform(Vector2f(0.0f, 0.18f), UiAnchor::LeftTop)),
	m_slider1(&m_section1.GetContent(), "Volume", 100.0f, 0.0f, 100.0f, 0, UiTransform(Vector2f(0.0f, 0.24f), UiAnchor::LeftTop)),
	m_text1(&m_section1.GetContent(), "Title", Window::Get()->GetTitle(), 14, UiTransform(Vector2f(0.0f, 0.30f), UiAnchor::LeftTop))
	//m_radio1(&m_section1.GetContent(), "First Value", UiInputRadio::Type::X, false, UiTransform(Vector2f(0.0f, 0.36f), UiAnchor::LeftTop)),
	//m_radio2(&m_section1.GetContent(), "Second Value", UiInputRadio::Type::X, true, UiTransform(Vector2f(0.0f, 0.405f), UiAnchor::LeftTop)),
	//m_radio3(&m_section1.GetContent(), "Third Value", UiInputRadio::Type::X, false, UiTransform(Vector2f(0.0f, 0.45f), UiAnchor::LeftTop)),
	//m_radioManager1(UiInputRadio::Type::Filled, false, { &m_radio1, &m_radio2, &m_radio3 })
{
	m_slider1.OnValue().Add([this](float value)
	{
		Audio::Get()->SetGain(Audio::Type::Effect, value);
	});
	m_text1.OnValue().Add([this](std::string text)
	{
		Window::Get()->SetTitle(text);
	});
}

void Inspector::UpdateObject()
{
	UiPanel::UpdateObject();
}
}
