#include "OverlayStartup.hpp"

#include "../devices/Display.hpp"
#include "../events/Events.hpp"
#include "../events/EventTime.hpp"
#include "../visual/DriverSlide.hpp"
#include "../visual/DriverConstant.hpp"

namespace Flounder
{
	OverlayStartup::OverlayStartup(UiObject *parent) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_guiBackground(new Gui(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f)), new Texture("res/guis/eg_background.png"), 1)),
		m_guiLogo(new Gui(this, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(0.4f, 0.4f)), new Texture("res/guis/equilibrium_games.png"), 1)),
		m_textCopyright(new Text(this, UiBound(Vector2(0.5f, 0.18f), "Centre", true), 1.4f, "Copyright (C) 2017, Equilibrium Games - All Rights Reserved. This product uses GLFW, Vulkan, and STB Image.", Uis::Get()->m_proximanova->GetRegular(), JustifyCentre, 0.9, 0.0012f, 0.024f)),
		m_starting(true)
	{
		Events::Get()->AddEvent(new EventTime(3.6f, false, [&]() // 1.649f
		{
			SetAlphaDriver(new DriverSlide(1.0f, 0.0f, 1.0f));
		}));
	}

	OverlayStartup::~OverlayStartup()
	{
		delete m_guiBackground;
		delete m_guiLogo;
		delete m_textCopyright;
	}

	void OverlayStartup::UpdateObject()
	{
		m_guiBackground->GetRectangle()->m_dimensions->m_x = Display::Get()->GetAspectRatio();
		m_guiBackground->SetScaleDriver(new DriverConstant(2.0f));
		m_guiBackground->SetVisible(true);
		m_guiLogo->SetVisible(true);
	}
}
