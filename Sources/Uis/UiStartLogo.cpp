#include "UiStartLogo.hpp"

#include "Maths/Visual/DriverSlide.hpp"

namespace acid
{
#if defined(ACID_VERBOSE)
const Time START_DELAY = Time::Seconds(1.0f);
#else
const Time START_DELAY = Time::Seconds(3.0f);
#endif

UiStartLogo::UiStartLogo(UiObject *parent) :
	UiObject(parent, UiBound::Screen),
	m_guiBackground(this, UiBound(Vector2f(0.5f, 0.5f), UiReference::Centre, UiAspect::Position, Vector2f(1.0f, 1.0f)), Image2d::Create("Guis/Black.png")),
	m_guiLogoAcid(this, UiBound(Vector2f(0.5f, 0.5f), UiReference::Centre, UiAspect::Position | UiAspect::Dimensions, Vector2f(0.4f, 0.4f)), Image2d::Create("Logos/Acid_01.png")),
	m_textCopyright(this, UiBound(Vector2f(0.5f, 0.8f), UiReference::Centre, UiAspect::Position | UiAspect::Dimensions), 1.8f,
		"Copyright (C) 2019, Equilibrium Games - All Rights Reserved.", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, 0.8f, Colour::White, 0.0012f,
		0.024f),
#if defined(ACID_VERBOSE)
	m_delayTimer(Time::Seconds(1.0f)),
#else
	m_delayTimer(Time::Seconds(3.0f)),
#endif
	m_fadeout(false),
	m_finished(false)
{
}

void UiStartLogo::UpdateObject()
{
	if (!m_fadeout && m_delayTimer.IsPassedTime())
	{
		m_delayTimer.ResetStartTime();
		m_fadeout = true;
		SetAlphaDriver(new DriverSlide<float>(1.0f, 0.0f, Time::Seconds(1.4f)));
	}

	if (GetScreenAlpha() <= 0.0f && !m_finished)
	{
		m_finished = true;
		m_onFinished();
	}
}
}
