#include "Hierarchy.hpp"

#include <Uis/Drivers/ConstantDriver.hpp>

namespace test {
int32_t GetNextY() {
	static uint32_t i = 0;
	return i++ * (UiInputButton::Size.m_y + 6);
}

Hierarchy::Hierarchy() {
	m_section1.SetTransform({{0.08f, 0.05f}, UiAnchor::LeftTop});
	m_section1.SetTitle("Section Left");
	UiPanel::AddChild(&m_section1);

	m_button1.SetTransform({{0.0f, 0.0f}, UiAnchor::LeftTop});
	m_button1.SetTitle("Button #1");
	m_button1.OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			Log::Out("Button #1 pressed!\n");
		}
	}, this);
	m_section1.AddChild(&m_button1);

	auto onSlide = [this](float value) {
		auto colour = Colour(m_sliderR.GetValue(), m_sliderG.GetValue(), m_sliderB.GetValue()) / 255.0f;
		dynamic_cast<ConstantDriver<Colour> *>(m_rgbColour.GetColourDriver())->SetConstant(colour);
		//m_colourWheel.SetValue(colour);
		m_textHex.SetValue(colour.GetHex());
	};

	m_sliderR.SetTransform({{0.0f, 0.06f}, UiAnchor::LeftTop});
	//m_sliderR.GetTransform().SetScale({0.071f, 0.042f});
	m_sliderR.SetTitle("R");
	m_sliderR.SetValueMin(0.0f);
	m_sliderR.SetValueMax(255.0f);
	m_sliderR.SetRoundTo(0);
	m_sliderR.SetValue(255.0f);
	m_sliderR.OnValue().Add(onSlide);
	m_section1.AddChild(&m_sliderR);

	m_sliderG.SetTransform({{0.074f, 0.06f}, UiAnchor::LeftTop});
	//m_sliderG.GetTransform().SetScale({0.071f, 0.042f});
	m_sliderG.SetTitle("G");
	m_sliderG.SetValueMin(0.0f);
	m_sliderG.SetValueMax(255.0f);
	m_sliderG.SetRoundTo(0);
	m_sliderG.SetValue(255.0f);
	m_sliderG.OnValue().Add(onSlide);
	m_section1.AddChild(&m_sliderG);

	m_sliderB.SetTransform({{0.1475f, 0.06f}, UiAnchor::LeftTop});
	//m_sliderB.GetTransform().SetScale({0.071f, 0.042f});
	m_sliderB.SetTitle("B");
	m_sliderR.SetValueMin(0.0f);
	m_sliderB.SetValueMax(255.0f);
	m_sliderB.SetRoundTo(0);
	m_sliderB.SetValue(0.0f);
	m_sliderB.OnValue().Add(onSlide);
	m_section1.AddChild(&m_sliderB);

	m_textHex.SetTransform({{0.0f, 0.12f}, UiAnchor::LeftTop});
	m_textHex.SetTitle("Hex");
	m_textHex.SetMaxLength(14);
	m_textHex.SetValue(Colour::Yellow.GetHex());
	m_section1.AddChild(&m_textHex);

	m_rgbColour.SetTransform({{0.0f, 0.18f}, UiAnchor::LeftTop, UiInputButton::Size});
	m_rgbColour.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	m_rgbColour.SetNinePatches({0.125f, 0.125f, 0.75f, 0.75f});
	m_rgbColour.SetColourDriver<ConstantDriver>(Colour::Yellow);
	m_section1.AddChild(&m_rgbColour);
	
	/*m_colourWheel.SetTransform({{0.0f, 0.24f}, UiAnchor::LeftTop});
	m_colourWheel.SetValue(Colour::Yellow);
	m_colourWheel.OnValue() += [this](Colour value) {
		m_sliderR.SetValue(255.0f * value.m_r);
		m_sliderG.SetValue(255.0f * value.m_g);
		m_sliderB.SetValue(255.0f * value.m_b);
		dynamic_cast<ConstantDriver<Colour> *>(m_rgbColour.GetColourDriver())->SetConstant(value);
	};
	m_colourWheel.AddChild(&m_button1);*/
	
	SetManipulate(UiManipulate::None);
	SetScrollBars(ScrollBar::None);
	SetBackgroundColor(UiInputButton::BackgroundColour);
}

void Hierarchy::UpdateObject() {
	UiPanel::UpdateObject();
}
}
