#include "Hierarchy.hpp"

#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
Hierarchy::Hierarchy(UiObject *parent) :
	UiPanel(parent, UiBound(Vector2f(0.0f, 0.0f), UiReference::TopLeft, UiAspect::Dimensions, Vector2f(0.3f, 1.0f)), ScrollBar::None),
	m_section1(&GetContent(), "Section Left", UiBound(Vector2f(0.08f, 0.05f), UiReference::TopLeft)),
	m_button1(&m_section1.GetContent(), "Button #1", UiBound(Vector2f(0.0f, 0.0f), UiReference::TopLeft)),
	m_sliderR(&m_section1.GetContent(), "R", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2f(0.0f, 0.06f), UiReference::TopLeft)),
	m_sliderG(&m_section1.GetContent(), "G", 255.0f, 0.0f, 255.0f, 0, UiBound(Vector2f(0.074f, 0.06f), UiReference::TopLeft)),
	m_sliderB(&m_section1.GetContent(), "B", 0.0f, 0.0f, 255.0f, 0, UiBound(Vector2f(0.1475f, 0.06f), UiReference::TopLeft)),
	m_textHex(&m_section1.GetContent(), "Hex", Colour::Yellow.GetHex(), 14, UiBound(Vector2f(0.0f, 0.12f), UiReference::TopLeft)),
	m_rgbColour(&m_section1.GetContent(), UiBound(Vector2f(0.0f, 0.18f), UiReference::TopLeft, UiAspect::Position | UiAspect::Dimensions, UiInputButton::Size),
		Image2d::Create("Guis/Button_Filled.png"), Colour::Yellow),
	m_colourWheel(&m_section1.GetContent(), Colour::Yellow, UiBound(Vector2f(0.0f, 0.24f), UiReference::TopLeft))
{
	m_rgbColour.SetNinePatches(Vector4f(0.125f, 0.125f, 0.75f, 0.75f));
	m_button1.OnClick() += [this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			Log::Out("Button #1 pressed!\n");
		}
	};

	m_sliderR.GetRectangle().SetDimensions(Vector2f(0.071f, 0.042f));
	m_sliderG.GetRectangle().SetDimensions(Vector2f(0.071f, 0.042f));
	m_sliderB.GetRectangle().SetDimensions(Vector2f(0.071f, 0.042f));
	auto onSlide = [this](float value)
	{
		auto colour = Colour(m_sliderR.GetValue(), m_sliderG.GetValue(), m_sliderB.GetValue()) / 255.0f;
		dynamic_cast<DriverConstant<Colour> *>(m_rgbColour.GetColourDriver())->SetConstant(colour);
		m_colourWheel.SetValue(colour);
		m_textHex.SetValue(colour.GetHex());
	};
	m_sliderR.OnValue() += onSlide;
	m_sliderG.OnValue() += onSlide;
	m_sliderB.OnValue() += onSlide;

	/*m_colourWheel.OnValue() += [this](Colour value) {
		m_sliderR.SetValue(255.0f * value.m_r);
		m_sliderG.SetValue(255.0f * value.m_g);
		m_sliderB.SetValue(255.0f * value.m_b);
		dynamic_cast<DriverConstant<Colour> *>(m_rgbColour.GetColourDriver())->SetConstant(value);
	};*/
}

void Hierarchy::UpdateObject()
{
	UiPanel::UpdateObject();
}
}
