#include "Hierarchy.hpp"

#include <Maths/Visual/DriverConstant.hpp>

namespace test
{
int32_t GetNextY()
{
	static uint32_t i = 0;
	return i++ * (UiInputButton::Size.m_y + 6);
}

Hierarchy::Hierarchy(UiObject *parent) :
	UiPanel{parent, {{400, 500}, UiAnchor::LeftTop}, UiInputButton::BackgroundColour, UiManipulate::None, ScrollBar::None},
	m_section1(&GetContent(), "Section Left", {{0.08f, 0.05f}, UiAnchor::LeftTop}),
	m_button1{&m_section1.GetContent(), "Button #1", {{0.0f, 0.0f}, UiAnchor::LeftTop}},
	m_sliderR{&m_section1.GetContent(), "R", 255.0f, 0.0f, 255.0f, 0, {{0.0f, 0.06f}, UiAnchor::LeftTop}},
	m_sliderG{&m_section1.GetContent(), "G", 255.0f, 0.0f, 255.0f, 0, {{0.074f, 0.06f}, UiAnchor::LeftTop}},
	m_sliderB{&m_section1.GetContent(), "B", 0.0f, 0.0f, 255.0f, 0, {{0.1475f, 0.06f}, UiAnchor::LeftTop}},
	m_textHex{&m_section1.GetContent(), "Hex", Colour::Yellow.GetHex(), 14, {{0.0f, 0.12f}, UiAnchor::LeftTop}},
	m_rgbColour{&m_section1.GetContent(), {{0.0f, 0.18f}, UiAnchor::LeftTop, UiInputButton::Size},
		Image2d::Create("Guis/Button_Filled.png"), Colour::Yellow}
	//m_colourWheel{&m_section1.GetContent(), Colour::Yellow, {{0.0f, 0.24f}, UiAnchor::LeftTop}}
{
	m_rgbColour.SetNinePatches({0.125f, 0.125f, 0.75f, 0.75f});
	m_button1.OnClick().Add([this](MouseButton button)
	{
		if (button == MouseButton::Left)
		{
			Log::Out("Button #1 pressed!\n");
		}
	});

	//m_sliderR.GetTransform().SetScale({0.071f, 0.042f});
	//m_sliderG.GetTransform().SetScale({0.071f, 0.042f});
	//m_sliderB.GetTransform().SetScale({0.071f, 0.042f});
	auto onSlide = [this](float value)
	{
		auto colour = Colour(m_sliderR.GetValue(), m_sliderG.GetValue(), m_sliderB.GetValue()) / 255.0f;
		dynamic_cast<DriverConstant<Colour> *>(m_rgbColour.GetColourDriver())->SetConstant(colour);
		//m_colourWheel.SetValue(colour);
		m_textHex.SetValue(colour.GetHex());
	};
	m_sliderR.OnValue().Add(onSlide);
	m_sliderG.OnValue().Add(onSlide);
	m_sliderB.OnValue().Add(onSlide);

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
