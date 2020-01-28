#include "UiStartLogo.hpp"

#include "Uis/Drivers/DriverSlide.hpp"
#include "Timers/Timers.hpp"

namespace acid {
#if defined(ACID_DEBUG)
const Time START_DELAY = 1s;
#else
const Time START_DELAY = 3s;
#endif

UiStartLogo::UiStartLogo(UiObject *parent) :
	UiObject(parent, {UiMargins::All}),
	m_guiBackground(this, {UiMargins::All}, Image2d::Create("Guis/Black.png")),
	m_guiLogoAcid(this, {{300, 300}, UiAnchor::Centre, {0, -100}}, Image2d::Create("Logos/Acid_01.png")),
	m_textCopyright(this, {{460, 64}, UiAnchor::Centre, {0, 128}}, 12.0f, "Copyright (C) 2019, Equilibrium Games - All Rights Reserved.",
		FontType::Create("Fonts/ProximaNova-Regular.fnt"), Text::Justify::Centre, Colour::White) {
	Timers::Get()->Once(START_DELAY, [this]() {
		SetAlphaDriver<DriverSlide>(1.0f, 0.0f, 1.4s);
	}, this);
}

void UiStartLogo::UpdateObject() {
	if (GetScreenAlpha() <= 0.0f && !m_finished) {
		m_finished = true;
		m_onFinished();
	}
}
}
