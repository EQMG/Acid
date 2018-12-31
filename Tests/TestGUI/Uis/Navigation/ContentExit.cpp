#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.45f, 0.5f), UiBound::CENTRE_LEFT, false, true, Vector2(0.8f, 0.8f))),
		m_background(std::make_unique<Gui>(this, GetRectangle(), Texture::Create("Guis/Black.png"))),
		m_button1(std::make_unique<UiInputButton>(this, "Test Button", UiBound(Vector2(0.65f, 0.79f), UiBound::CENTRE, false, true, UiNavigation::BUTTON_SIZE), UiNavigation::BUTTON_COLOUR)),
		m_input1(std::make_unique<UiInputGrabber>(this, "Test Grabber: ", MOUSE_BUTTON_1, new UiGrabberMouse(), UiBound(Vector2(0.65f, 0.72f), UiBound::CENTRE, false, true, UiNavigation::BUTTON_SIZE), UiNavigation::BUTTON_COLOUR)),
		m_slider1(std::make_unique<UiInputSlider>(this, "Test Slider: ", 100.0f, 0.0f, 100.0f, 1, UiBound(Vector2(0.65f, 0.65f), UiBound::CENTRE, false, true, UiNavigation::BUTTON_SIZE), UiNavigation::BUTTON_COLOUR)),
		m_text1(std::make_unique<UiInputText>(this, "Test Text: ", "Hello World", 14, UiBound(Vector2(0.65f, 0.58f), UiBound::CENTRE, false, true, UiNavigation::BUTTON_SIZE), UiNavigation::BUTTON_COLOUR))
	{
		m_background->SetColourOffset(UiNavigation::PANEL_COLOUR); // TODO: Blur underneath.
	//	m_background->SetDepth(1.0f);
		m_slider1->GetOnSlide() += [](UiInputSlider *object, float value) {
			Audio::Get()->SetTypeGain(SOUND_TYPE_EFFECT, value);
		};
	}

	void ContentExit::UpdateObject()
	{
	}
}
