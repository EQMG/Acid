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
	UiObject(parent, UiTransform::Screen),
	m_guiBackground(this, UiTransform(Vector2f(0.5f, 0.5f), UiAnchor::Centre, UiAspect::Position, Vector2f(1.0f, 1.0f)), Image2d::Create("Guis/Black.png")),
	m_guiLogoAcid(this, UiTransform(Vector2f(0.5f, 0.5f), UiAnchor::Centre, UiAspect::Position | UiAspect::Size, Vector2f(0.4f, 0.4f)), Image2d::Create("Logos/Acid_01.png")),
	m_textCopyright(this, UiTransform(Vector2f(0.5f, 0.8f), UiAnchor::Centre, UiAspect::Position | UiAspect::Size), 1.8f,
		"Copyright (C) 2019, Equilibrium Games - All Rights Reserved.", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, 0.8f, Colour::White, 0.0012f,
		0.024f),
	m_finished(false)
{
	Timers::Get()->Once(START_DELAY, [this]()
	{
		SetAlphaDriver(new DriverSlide<float>(1.0f, 0.0f, Time::Seconds(1.4f)));
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
