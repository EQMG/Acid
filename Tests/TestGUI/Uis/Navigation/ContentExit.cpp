#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.45f, 0.5f), UiBound::CentreLeft, false, true, Vector2(0.8f, 0.8f))),
		m_background(std::make_unique<Gui>(this, GetRectangle(), Texture::Create("Guis/White.png"))),
		m_button1(std::make_unique<UiInputButton>(this, "Test Button", UiBound(Vector2(0.65f, 0.17f), UiBound::Centre, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_input1(std::make_unique<UiInputGrabber>(this, "Test Grabber: ", MOUSE_BUTTON_1, new UiInputGrabber::GrabberMouse(), UiBound(Vector2(0.65f, 0.24f), UiBound::Centre, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_slider1(std::make_unique<UiInputSlider>(this, "Test Slider: ", 100.0f, 0.0f, 100.0f, 1, UiBound(Vector2(0.65f, 0.31f), UiBound::Centre, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_text1(std::make_unique<UiInputText>(this, "Test Text: ", "Hello World", 14, UiBound(Vector2(0.65f, 0.38f), UiBound::Centre, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton))
	{
		m_background->SetColourOffset(UiNavigation::ColourPanel); // TODO: Blur underneath.
	//	m_background->SetDepth(1.0f);
		m_slider1->GetOnSlide() += [](UiInputSlider *object, float value) {
			Audio::Get()->SetTypeGain(Audio::Type::Effect, value);
		};
	}

	void ContentExit::UpdateObject()
	{
	}
}
