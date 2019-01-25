#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.45f, 0.5f), UiBound::CentreLeft, false, true, Vector2(0.8f, 0.8f))),
		m_background(std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 0.0f), UiBound::TopLeft, false, true, Vector2(0.8f, 0.8f)), Texture::Create("Guis/White.png"))),
		m_button1(std::make_unique<UiInputButton>(this, "Test Button", UiBound(Vector2(0.1f, 0.06f), UiBound::TopLeft, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_input1(std::make_unique<UiInputGrabber>(this, "Test Grabber: ", MOUSE_BUTTON_1, new UiInputGrabber::GrabberMouse(), UiBound(Vector2(0.1f, 0.13f), UiBound::TopLeft, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_slider1(std::make_unique<UiInputSlider>(this, "Test Slider: ", 100.0f, 0.0f, 100.0f, 1, UiBound(Vector2(0.1f, 0.2f), UiBound::TopLeft, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_text1(std::make_unique<UiInputText>(this, "Test Text: ", "Hello World", 14, UiBound(Vector2(0.1f, 0.27f), UiBound::TopLeft, false, true, UiNavigation::ButtonSize), UiNavigation::ColourButton))
	{
		/*auto guiA = new Gui(Uis::Get()->GetContainer(), UiBound(Vector2(1.0f, 0.0f), UiBound::TopRight, true, true, Vector2(0.3f, 1.0f)), Texture::Create("Guis/White.png"));
		guiA->SetDepth(1.0f);
		guiA->SetColour(Colour::Teal);

		auto guiB = new Gui(guiA, UiBound(Vector2(0.5f, 0.5f), UiBound::Centre, true, true, Vector2(0.25f, 0.25f)), Texture::Create("Guis/White.png"));
		guiB->SetDepth(2.0f);
		guiB->SetColour(Colour::Red);*/

		m_background->SetColour(UiNavigation::ColourPanel); // TODO: Blur underneath.
	//	m_background->SetDepth(1.0f);
		m_slider1->GetOnSlide() += [](UiInputSlider *object, float value) {
			Audio::Get()->SetTypeGain(Audio::Type::Effect, value);
		};
	}

	void ContentExit::UpdateObject()
	{
		//GetRectangle().SetPosition(Mouse::Get()->GetPosition());
	}
}
