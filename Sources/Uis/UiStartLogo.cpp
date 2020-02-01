#include "UiStartLogo.hpp"

#include "Uis/Drivers/SlideDriver.hpp"
#include "Timers/Timers.hpp"

namespace acid {
#if defined(ACID_DEBUG)
const Time START_DELAY = 1s;
#else
const Time START_DELAY = 3s;
#endif

UiStartLogo::UiStartLogo() {
	m_guiBackground.SetTransform({UiMargins::All});
	m_guiBackground.SetImage(Image2d::Create("Guis/Black.png"));
	AddChild(&m_guiBackground);

	m_guiLogoAcid.SetTransform({{300, 300}, UiAnchor::Centre, {0, -100}});
	m_guiLogoAcid.SetImage(Image2d::Create("Logos/Acid_01.png"));
	AddChild(&m_guiLogoAcid);

	m_textCopyright.SetTransform({{460, 64}, UiAnchor::Centre, {0, 128}});
	m_textCopyright.SetFontType(FontType::Create("Fonts/ProximaNova-Regular.ttf"));
	m_textCopyright.SetJustify(Text::Justify::Centre);
	m_textCopyright.SetTextColour(Colour::White);
	m_textCopyright.SetString("Copyright (C) 2019, Equilibrium Games - All Rights Reserved.");
	AddChild(&m_textCopyright);

	Timers::Get()->Once(START_DELAY, [this]() {
		SetAlphaDriver<SlideDriver>(1.0f, 0.0f, 1.4s);
	}, this);
}

void UiStartLogo::UpdateObject() {
	if (GetScreenAlpha() <= 0.0f && !m_finished) {
		m_finished = true;
		m_onFinished();
	}
}
}
