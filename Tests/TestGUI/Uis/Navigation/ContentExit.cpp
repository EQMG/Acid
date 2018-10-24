#include "ContentExit.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.4375f), "Centre", true, false, Vector2(0.8f, 0.7f))),
		m_background(std::make_unique<Gui>(this, GetRectangle(), Texture::Resource("Guis/White.png"))),
		m_button1(std::make_unique<UiInputButton>(m_background.get(), Vector2(0.35f, 0.4375f), "Test Button")),
		m_input1(std::make_unique<UiInputGrabber>(m_background.get(), Vector2(0.35f, 0.38f), "Test Grabber: ", MOUSE_BUTTON_1, new UiGrabberMouse())),
		m_slider1(std::make_unique<UiInputSlider>(m_background.get(), Vector2(0.65f, 0.4375f), "Test Slider: ", 1, 0.0f, 100.0f, 50.0f)),
		m_text1(std::make_unique<UiInputText>(m_background.get(), Vector2(0.65f, 0.38f), "Test Text: ", "Hello World", 14))
	{
		m_slider1->SetActionChange([](float value) {
			Audio::Get()->SetVolume(SOUND_TYPE_EFFECT, value);
		});
	}

	void ContentExit::UpdateObject()
	{
	}
}
