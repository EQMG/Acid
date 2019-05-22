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
	UiObject(parent, UiTransform(Window::Get()->GetSize())),
	m_guiBackground(this, UiTransform(Window::Get()->GetSize()), Image2d::Create("Guis/Black.png")),
	m_guiLogoAcid(this, UiTransform(Vector2i(300, 300), Vector2i(0, -128), UiAnchor::Centre), Image2d::Create("Logos/Acid_01.png")),
	m_textCopyright(this, UiTransform(Vector2i(600, 128), Vector2i(0, 50), UiAnchor::Centre), 12,
		"Copyright (C) 2019, Equilibrium Games - All Rights Reserved.", FontType::Create("Fonts/ProximaNova", "Regular"), Text::Justify::Centre, Colour::White), // , 0.0012f, 0.024f
	m_finished(false)
{
	Timers::Get()->Once(START_DELAY, [this]()
	{
		SetAlphaDriver(new DriverSlide<float>(1.0f, 0.0f, Time::Seconds(1.4f)));
	}, this);
}

void UiStartLogo::UpdateObject()
{
	GetTransform().SetSize(Window::Get()->GetSize());
	m_guiBackground.GetTransform().SetSize(Window::Get()->GetSize());

	if (GetScreenAlpha() <= 0.0f && !m_finished)
	{
		m_finished = true;
		m_onFinished();
	}
}
}
