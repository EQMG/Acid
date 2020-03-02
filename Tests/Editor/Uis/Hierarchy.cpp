#include "Hierarchy.hpp"

#include <Uis/Drivers/ConstantDriver.hpp>

namespace test {
int32_t GetNextY() {
	static uint32_t i = 0;
	return i++ * (UiInputButton::Size.y + 6);
}

Hierarchy::Hierarchy() {
	//section1.SetTransform({{0.08f, 0.05f}, UiAnchor::LeftTop});
	section1.SetTitle("Section Left");
	UiPanel::AddChild(&section1);

	//button1.SetTransform({{0.0f, 0.0f}, UiAnchor::LeftTop});
	button1.SetTitle("Button #1");
	button1.OnClick().Add([this](MouseButton button) {
		if (button == MouseButton::Left) {
			Log::Out("Button #1 pressed!\n");
		}
	}, this);
	section1.AddChild(&button1);

	auto onSlide = [this](float value) {
		auto colour = Colour(sliderR.GetValue(), sliderG.GetValue(), sliderB.GetValue()) / 255.0f;
		dynamic_cast<ConstantDriver<Colour> *>(rgbColour.GetColourDriver())->SetConstant(colour);
		//colourWheel.SetValue(colour);
		textHex.SetValue(colour.GetHex());
	};

	//sliderR.SetTransform({{0.0f, 0.06f}, UiAnchor::LeftTop});
	//sliderR.GetTransform().SetScale({0.071f, 0.042f});
	sliderR.SetTitle("R");
	sliderR.SetValueMin(0.0f);
	sliderR.SetValueMax(255.0f);
	sliderR.SetRoundTo(0);
	sliderR.SetValue(255.0f);
	sliderR.OnValue().Add(onSlide);
	section1.AddChild(&sliderR);

	//sliderG.SetTransform({{0.074f, 0.06f}, UiAnchor::LeftTop});
	//sliderG.GetTransform().SetScale({0.071f, 0.042f});
	sliderG.SetTitle("G");
	sliderG.SetValueMin(0.0f);
	sliderG.SetValueMax(255.0f);
	sliderG.SetRoundTo(0);
	sliderG.SetValue(255.0f);
	sliderG.OnValue().Add(onSlide);
	section1.AddChild(&sliderG);

	//sliderB.SetTransform({{0.1475f, 0.06f}, UiAnchor::LeftTop});
	//sliderB.GetTransform().SetScale({0.071f, 0.042f});
	sliderB.SetTitle("B");
	sliderR.SetValueMin(0.0f);
	sliderB.SetValueMax(255.0f);
	sliderB.SetRoundTo(0);
	sliderB.SetValue(0.0f);
	sliderB.OnValue().Add(onSlide);
	section1.AddChild(&sliderB);

	//textHex.SetTransform({{0.0f, 0.12f}, UiAnchor::LeftTop});
	textHex.SetTitle("Hex");
	textHex.SetMaxLength(14);
	textHex.SetValue(Colour::Yellow.GetHex());
	section1.AddChild(&textHex);

	//rgbColour.SetTransform({{0.0f, 0.18f}, UiAnchor::LeftTop, UiInputButton::Size});
	rgbColour.SetImage(Image2d::Create("Guis/Button_Filled.png"));
	rgbColour.SetNinePatches({0.125f, 0.125f, 0.75f, 0.75f});
	rgbColour.SetColourDriver<ConstantDriver>(Colour::Yellow);
	section1.AddChild(&rgbColour);
	
	/*colourWheel.SetTransform({{0.0f, 0.24f}, UiAnchor::LeftTop});
	colourWheel.SetValue(Colour::Yellow);
	colourWheel.OnValue() += [this](Colour value) {
		sliderR.SetValue(255.0f * value.r);
		sliderG.SetValue(255.0f * value.g);
		sliderB.SetValue(255.0f * value.b);
		dynamic_cast<ConstantDriver<Colour> *>(rgbColour.GetColourDriver())->SetConstant(value);
	};
	colourWheel.AddChild(&button1);*/
	
	SetManipulate(UiManipulate::None);
	SetScrollBars(ScrollBar::None);
	SetBackgroundColor(UiInputButton::BackgroundColour);
}

void Hierarchy::UpdateObject() {
	UiPanel::UpdateObject();
}
}
