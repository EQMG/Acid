#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.45f, 0.5f), UiReference::CentreLeft, UiAspect::Dimensions, Vector2(0.8f, 0.8f))),
		m_background(std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Dimensions, Vector2(0.8f, 0.8f)), Texture::Create("Guis/White.png"), UiNavigation::ColourPanel)),
		m_button1(std::make_unique<UiInputButton>(this, "Button #1", UiBound(Vector2(0.08f, 0.05f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_button2(std::make_unique<UiInputButton>(this, "Button #2", UiBound(Vector2(0.92f, 0.05f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_input1(std::make_unique<UiInputGrabber>(this, "Mouse Grabber: ", MOUSE_BUTTON_1, new UiInputGrabber::GrabberMouse(), UiBound(Vector2(0.08f, 0.15f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_slider1(std::make_unique<UiInputSlider>(this, "Volume Slider: ", 100.0f, 0.0f, 100.0f, 1, UiBound(Vector2(0.08f, 0.25f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, UiNavigation::ButtonSize), UiNavigation::ColourButton)),
		m_text1(std::make_unique<UiInputText>(this, "Window Title: ", Window::Get()->GetTitle(), 14, UiBound(Vector2(0.08f, 0.35f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, UiNavigation::ButtonSize), UiNavigation::ColourButton))
	{
	//	m_background->SetDepth(1.0f); // TODO: Blur underneath.

		m_button1->GetOnPressed() += [](UiInputButton *object, bool pressed) {
			Log::Out("Button #1 says hello!\n");
		};
		m_button2->GetOnPressed() += [](UiInputButton *object, bool pressed) {
			Log::Out("Button #2 says hello!\n");
		};
		m_slider1->GetOnSlide() += [](UiInputSlider *object, float value) {
			Audio::Get()->SetTypeGain(Audio::Type::Effect, value);
		};
		m_text1->GetOnType() += [](UiInputText *object, std::string text) {
			Window::Get()->SetTitle(text);
		};
	}

	void ContentExit::UpdateObject()
	{
		//auto d = GetRectangle().GetDimensions();
		//d.m_x = Mouse::Get()->GetPosition().m_x;
		//GetRectangle().SetDimensions(d);

		//GetRectangle().SetPosition(Mouse::Get()->GetPosition());
	}
}
