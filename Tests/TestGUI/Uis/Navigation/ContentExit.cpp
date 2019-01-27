#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include "ContentExit.hpp"

#include "UiNavigation.hpp"

namespace test
{
	ContentExit::ContentExit(UiObject *parent) :
		UiPanel(parent, UiBound(Vector2(0.45f, 0.5f), UiReference::CentreLeft, UiAspect::Dimensions, Vector2(0.8f, 0.8f))), // , ScrollBar::None
		m_button1(std::make_unique<UiInputButton>(GetContent(), "Button #1", UiBound(Vector2(0.08f, 0.05f), UiReference::TopLeft))),
		m_button2(std::make_unique<UiInputButton>(GetContent(), "Button #2", UiBound(Vector2(0.92f, 0.05f), UiReference::TopRight))),
		m_button3(std::make_unique<UiInputButton>(GetContent(), "Button #3", UiBound(Vector2(1.02f, 0.05f), UiReference::TopLeft))),
		m_radio1(std::make_unique<UiInputRadio>(GetContent(), "First Value", UiInputRadio::Mark::X, false, UiBound(Vector2(0.08f, 0.47f), UiReference::TopLeft))),
		m_radio2(std::make_unique<UiInputRadio>(GetContent(), "Second Value", UiInputRadio::Mark::X, false, UiBound(Vector2(0.08f, 0.5225f), UiReference::TopLeft))),
		m_radio3(std::make_unique<UiInputRadio>(GetContent(), "Third Value", UiInputRadio::Mark::X, true, UiBound(Vector2(0.08f, 0.575f), UiReference::TopLeft))),
		m_radioManager(UiRadioManager(UiInputRadio::Mark::Fill, false, {m_radio1.get(), m_radio2.get(), m_radio3.get()})),
		m_image1(std::make_unique<Gui>(GetContent(), UiBound(Vector2(1.02f, 0.12f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, Vector2(0.3f, 1.0f)), Texture::Create("Guis/Gradient.png"))),
		m_input1(std::make_unique<UiGrabberMouse>(GetContent(), "Mouse Grabber", MouseButton::Left, UiBound(Vector2(0.08f, 0.12f), UiReference::TopLeft))),
		m_input2(std::make_unique<UiGrabberKeyboard>(GetContent(), "Keyboard Grabber", Key::W, UiBound(Vector2(0.08f, 0.19f), UiReference::TopLeft))),
		m_input3(std::make_unique<UiGrabberJoystick>(GetContent(), "Joystick Grabber", 0, 3, UiBound(Vector2(0.08f, 0.26f), UiReference::TopLeft))),
		m_slider1(std::make_unique<UiInputSlider>(GetContent(), "Volume", 100.0f, 0.0f, 100.0f, 0, UiBound(Vector2(0.08f, 0.33f), UiReference::TopLeft))),
		m_text1(std::make_unique<UiInputText>(GetContent(), "Title", Window::Get()->GetTitle(), 14, UiBound(Vector2(0.08f, 0.4f), UiReference::TopLeft))),
		m_sliderR(std::make_unique<UiInputSlider>(GetContent(), "R", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.92f, 0.12f), UiReference::TopRight))),
		m_sliderG(std::make_unique<UiInputSlider>(GetContent(), "G", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.92f, 0.19f), UiReference::TopRight))),
		m_sliderB(std::make_unique<UiInputSlider>(GetContent(), "B", 0.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.92f, 0.26f), UiReference::TopRight))),
		m_colourWheel(std::make_unique<UiColourWheel>(GetContent(), Colour::Purple, UiBound(Vector2(0.92f, 0.33f), UiReference::TopRight))),
		m_rgbColour(std::make_unique<Gui>(GetContent(), UiBound(Vector2(0.92f, 0.72f), UiReference::TopRight, UiAspect::Position | UiAspect::Dimensions, Vector2(0.3f, 0.045f)), Texture::Create("Guis/Button_Full.png"), Colour::Yellow))
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

		auto onSlide = [this](UiInputSlider *object, float value) {
			auto colour = Colour(m_sliderR->GetValue(), m_sliderG->GetValue(), m_sliderB->GetValue());
			dynamic_cast<DriverConstant<Colour> *>(m_rgbColour->GetColourDriver())->SetConstant(colour);
			m_colourWheel->SetColour(colour);
		};
		m_sliderR->GetOnSlide() += onSlide;
		m_sliderG->GetOnSlide() += onSlide;
		m_sliderB->GetOnSlide() += onSlide;

		m_colourWheel->GetOnPressed() += [this](UiColourWheel *object, Colour value) {
			m_sliderR->SetValue(255.0f * value.m_r);
			m_sliderG->SetValue(255.0f * value.m_g);
			m_sliderB->SetValue(255.0f * value.m_b);
			dynamic_cast<DriverConstant<Colour> *>(m_rgbColour->GetColourDriver())->SetConstant(value);
		};
	}

	void ContentExit::UpdateObject()
	{
		UiPanel::UpdateObject();
	}
}
