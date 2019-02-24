#include "Hierarchy.hpp"

#include <Devices/Keyboard.hpp>
#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
	Hierarchy::Hierarchy(UiObject *parent) :
		UiPanel(parent, UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Dimensions, Vector2(0.3f, 1.0f)), ScrollBar::None),
		m_section1(std::make_unique<UiSection>(GetContent(), "Section Left", UiBound(Vector2(0.08f, 0.05f), UiReference::TopLeft))),
		m_button1(std::make_unique<UiInputButton>(m_section1->GetContent(), "Button #1", UiBound(Vector2(0.0f, 0.0f), UiReference::TopLeft))),
		m_sliderR(std::make_unique<UiInputSlider>(m_section1->GetContent(), "R", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.0f, 0.06f), UiReference::TopLeft))),
		m_sliderG(std::make_unique<UiInputSlider>(m_section1->GetContent(), "G", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.074f, 0.06f), UiReference::TopLeft))),
		m_sliderB(std::make_unique<UiInputSlider>(m_section1->GetContent(), "B", 0.0f, 0.0f, 255.0f, 0, UiBound(Vector2(0.1475f, 0.06f), UiReference::TopLeft))),
		m_textHex(std::make_unique<UiInputText>(m_section1->GetContent(), "Hex", Colour::Yellow.GetHex(), 14, UiBound(Vector2(0.0f, 0.12f), UiReference::TopLeft))),
		m_rgbColour(std::make_unique<Gui>(m_section1->GetContent(), UiBound(Vector2(0.0f, 0.18f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, UiInputButton::Size),
			Texture::Create("Guis/Button_Filled.png"), Colour::Yellow)),
		m_colourWheel(std::make_unique<UiColourWheel>(m_section1->GetContent(), Colour::Yellow, UiBound(Vector2(0.0f, 0.24f), UiReference::TopLeft)))
	{
		m_rgbColour->SetNinePatches(Vector4(0.125f, 0.125f, 0.75f, 0.75f));
		m_button1->GetOnClick() += [](UiObject *object, MouseButton button)
		{
			if (button == MouseButton::Left)
			{
				Log::Out("Button #1 pressed!\n");
			}
		};

		m_sliderR->GetRectangle().SetDimensions(Vector2(0.071f, 0.042f));
		m_sliderG->GetRectangle().SetDimensions(Vector2(0.071f, 0.042f));
		m_sliderB->GetRectangle().SetDimensions(Vector2(0.071f, 0.042f));
		auto onSlide = [this](UiInputSlider *object, float value)
		{
			auto colour = Colour(m_sliderR->GetValue(), m_sliderG->GetValue(), m_sliderB->GetValue()) / 255.0f;
			dynamic_cast<DriverConstant<Colour> *>(m_rgbColour->GetColourDriver())->SetConstant(colour);
			m_colourWheel->SetColour(colour);
			m_textHex->SetValue(colour.GetHex());
		};
		m_sliderR->GetOnSlide() += onSlide;
		m_sliderG->GetOnSlide() += onSlide;
		m_sliderB->GetOnSlide() += onSlide;

		/*m_colourWheel->GetOnPressed() += [this](UiColourWheel *object, Colour value) {
			m_sliderR->SetValue(255.0f * value.m_r);
			m_sliderG->SetValue(255.0f * value.m_g);
			m_sliderB->SetValue(255.0f * value.m_b);
			dynamic_cast<DriverConstant<Colour> *>(m_rgbColour->GetColourDriver())->SetConstant(value);
		};*/
	}

	void Hierarchy::UpdateObject()
	{
		UiPanel::UpdateObject();
	}
}
