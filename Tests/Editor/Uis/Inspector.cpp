#include "Inspector.hpp"

#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	Inspector::Inspector(UiObject *parent) :
		UiPanel(parent, UiBound(Vector2(1.0f, 0.0f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions, Vector2(0.3f, 1.0f)), ScrollBar::None),
		m_section1(&GetContent(), "Section Right", UiBound(Vector2(0.08f, 0.05f), UiReference::TopLeft)),
		m_button1(&m_section1.GetContent(), "Button #2", UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft)),
		m_input1(&m_section1.GetContent(), "Mouse Grabber", MouseButton::Left, UiBound(Vector2(0.0f, 0.06f), UiReference::TopLeft)),
		m_input2(&m_section1.GetContent(), "Keyboard Grabber", Key::W, UiBound(Vector2(0.0f, 0.12f), UiReference::TopLeft)),
		m_input3(&m_section1.GetContent(), "Joystick Grabber", 0, 3, UiBound(Vector2(0.0f, 0.18f), UiReference::TopLeft)),
		m_slider1(&m_section1.GetContent(), "Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.0f, 0.24f), UiReference::TopLeft)),
		m_text1(&m_section1.GetContent(), "Title", Window::Get()->GetTitle(), 14, UiBound(Vector2(0.0f, 0.30f), UiReference::TopLeft)),
		m_radio1(&m_section1.GetContent(), "First Value", UiInputRadio::Mark::X, false, UiBound(Vector2(0.0f, 0.36f), UiReference::TopLeft)),
		m_radio2(&m_section1.GetContent(), "Second Value", UiInputRadio::Mark::X, true, UiBound(Vector2(0.0f, 0.405f), UiReference::TopLeft)),
		m_radio3(&m_section1.GetContent(), "Third Value", UiInputRadio::Mark::X, false, UiBound(Vector2(0.0f, 0.45f), UiReference::TopLeft)),
		m_radioManager1(UiInputRadio::Mark::Filled, false, {&m_radio1, &m_radio2, &m_radio3})
	{
		m_slider1.GetOnSlide() += [](UiInputSlider *object, float value)
		{
			Audio::Get()->SetTypeGain(Audio::Type::Effect, value);
		};
		m_text1.GetOnType() += [](UiInputText *object, std::string text)
		{
			Window::Get()->SetTitle(text);
		};
	}

	void Inspector::UpdateObject()
	{
		UiPanel::UpdateObject();
	}
}
