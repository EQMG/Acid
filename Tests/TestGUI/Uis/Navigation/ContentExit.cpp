#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.45f, 0.5f), UiReference::CentreLeft, UiAspect::Dimensions, Vector2(0.8f, 0.8f))),
		m_background(std::make_unique<Gui>(this, UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Dimensions, Vector2(0.8f, 0.8f)), Texture::Create("Guis/Panel.png"), UiNavigation::ColourPanel)),
		m_button1(std::make_unique<UiInputButton>(this, "Button #1", UiBound(Vector2(0.08f, 0.05f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions))),
		m_button2(std::make_unique<UiInputButton>(this, "Button #2", UiBound(Vector2(0.92f, 0.05f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions))),
		m_input1(std::make_unique<UiGrabberMouse>(this, "Mouse Grabber", MouseButton::Left, UiBound(Vector2(0.08f, 0.12f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions))),
		m_input2(std::make_unique<UiGrabberKeyboard>(this, "Keyboard Grabber", Key::W, UiBound(Vector2(0.08f, 0.19f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions))),
		m_input3(std::make_unique<UiGrabberJoystick>(this, "Joystick Grabber", 0, 3, UiBound(Vector2(0.08f, 0.26f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions))),
		m_slider1(std::make_unique<UiInputSlider>(this, "Volume", 100.0f, 0.0f, 100.0f, 1, UiBound(Vector2(0.08f, 0.33f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions))),
		m_text1(std::make_unique<UiInputText>(this, "Title", Window::Get()->GetTitle(), 14, UiBound(Vector2(0.08f, 0.4f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions))),
		m_sliderR(std::make_unique<UiInputSlider>(this, "R", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.92f, 0.12f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions))),
		m_sliderG(std::make_unique<UiInputSlider>(this, "G", 255.0f, 0.0f, 255.0f, 1, UiBound(Vector2(0.92f, 0.19f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions))),
		m_sliderB(std::make_unique<UiInputSlider>(this, "B", 0.0f, 0.0f, 255.0f, 1, UiBound(Vector2(0.92f, 0.26f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions))),
		m_rgbColour(std::make_unique<Gui>(this, UiBound(Vector2(0.92f, 0.33f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions, Vector2(0.3f, 0.045f)), Texture::Create("Guis/Button.png"), Colour::Yellow))
	{
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

		auto onSlide = [&](UiInputSlider *slider, float value) {
			dynamic_cast<DriverConstant<Colour> *>(m_rgbColour->GetColourDriver())->SetConstant(Colour(m_sliderR->GetValue(), m_sliderG->GetValue(), m_sliderB->GetValue()));
		};
		m_sliderR->GetOnSlide() += onSlide;
		m_sliderG->GetOnSlide() += onSlide;
		m_sliderB->GetOnSlide() += onSlide;
	}

	void ContentExit::UpdateObject()
	{
	}
}
