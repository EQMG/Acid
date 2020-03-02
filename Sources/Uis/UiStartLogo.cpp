#include "UiStartLogo.hpp"

#include "Constraints/PixelConstraint.hpp"
#include "Constraints/RatioConstraint.hpp"
#include "Drivers/SlideDriver.hpp"
#include "Timers/Timers.hpp"

namespace acid {
UiStartLogo::UiStartLogo() {
	//background.SetTransform({UiMargins::All});
	background.SetImage(Image2d::Create("Guis/Black.png"));
	AddChild(&background);

	//logoAcid.SetTransform({{300, 300}, UiAnchor::Centre, {0, -100}});
	logoAcid.GetConstraints().SetWidth<PixelConstraint>(300)
		.SetHeight<RatioConstraint>(1.0f)
		.SetX<PixelConstraint>(0, UiAnchor::Centre)
		.SetY<PixelConstraint>(-100, UiAnchor::Centre);
	logoAcid.SetImage(Image2d::Create("Logos/Acid_01.png"));
	AddChild(&logoAcid);

	//textCopyright.SetTransform({{460, 64}, UiAnchor::Centre, {0, 128}});
	textCopyright.GetConstraints().SetWidth<PixelConstraint>(460)
		.SetHeight<PixelConstraint>(64)
		.SetX<PixelConstraint>(0, UiAnchor::Centre)
		.SetY<PixelConstraint>(128, UiAnchor::Centre);
	textCopyright.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	textCopyright.SetJustify(Text::Justify::Centre);
	textCopyright.SetTextColour(Colour::White);
	textCopyright.SetString("Copyright (C) 2019, Equilibrium Games - All Rights Reserved.");
	AddChild(&textCopyright);

	Timers::Get()->Once(StartDelay, [this]() {
		SetAlphaDriver<SlideDriver>(1.0f, 0.0f, 1.4s);
	}, this);
}

void UiStartLogo::UpdateObject() {
	if (GetScreenAlpha() <= 0.0f && !finished) {
		finished = true;
		onFinished();
	}
}
}
