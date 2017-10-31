#include "OverlayStartup.hpp"

#include "../devices/Display.hpp"
#include "../events/Events.hpp"
#include "../events/EventTime.hpp"
#include "../visual/DriverSlide.hpp"

namespace Flounder
{
	OverlayStartup::OverlayStartup(UiObject *parent) :
		UiObject(parent, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f)),
		m_guiBackground(new Gui(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(1.0f, 1.0f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("res/guis/eg_background.png"), 1)),
		m_guiLogo(new Gui(this, Vector3(0.5f, 0.5f, RelativeScreen), Vector3(0.4f, 0.4f, RelativeScreen), Vector2(0.5f, 0.5f), new Texture("res/guis/equilibrium_games.png"), 1)),
		m_textCopyright(new Text(this, Vector3(0.5f, 0.18f, RelativeScreen), 2.6f, Vector2(0.5f, 0.5f), "Copyright (C) 2017, Equilibrium Games - All Rights Reserved. This product uses GLFW, Vulkan, and STB Image.", Uis::Get()->m_cafeFrancoise->GetRegular(), JustifyCentre, 0.5f)),
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
		m_guiBackground->GetDimensions()->m_x = Display::Get()->GetAspectRatio();
		m_guiBackground->SetVisible(true);
		m_guiLogo->SetVisible(true);
	}
}
