#include "UiStartLogo.hpp"

#include "Maths/Visual/DriverSlide.hpp"
#include "Timers/Timers.hpp"

namespace acid
{
#if defined(ACID_VERBOSE)
const Time START_DELAY = 1s;
#else
const Time START_DELAY = 3s;
#endif

UiStartLogo::UiStartLogo(UiObject *parent) :
	UiObject{parent, UiTransform{UiMargins::All}},
	m_guiBackground{this, UiTransform{UiMargins::All}, Image2d::Create("Guis/Black.png")},
	m_guiLogoAcid{this, UiTransform{Vector2i{300, 300}, UiAnchor::Centre, Vector2i{0, -100}}, Image2d::Create("Logos/Acid_01.png")},
	m_textCopyright{this, UiTransform{Vector2i{460, 64}, UiAnchor::Centre, Vector2i{0, 128}}, 12.0f,
	"Copyright (C) 2019, Equilibrium Games - All Rights Reserved.", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, Colour::White}
{
	Timers::Get()->Once(START_DELAY, [this]()
	{
		SetAlphaDriver(std::make_unique<DriverSlide<float>>(1.0f, 0.0f, Time::Seconds(1.4f)));
	}, this);
}

void UiStartLogo::UpdateObject()
{
	if (GetScreenAlpha() <= 0.0f && !m_finished)
	{
		m_finished = true;
		m_onFinished();
	}
}
}
