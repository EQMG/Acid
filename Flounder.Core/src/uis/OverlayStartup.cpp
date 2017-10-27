#include "OverlayStartup.hpp"

#include "../devices/Display.hpp"
#include "../events/Events.hpp"
#include "../events/EventTime.hpp"
#include "../visual/DriverSlide.hpp"

namespace Flounder
{
	OverlayStartup::OverlayStartup(UiObject *parent) :
		UiObject(parent, Vector3(0.5f, 0.5f, true), Vector3(1.0f, 1.0f, true), Vector2(0.5f, 0.5f)),
		m_guiBackground(new Gui(this, Vector3(0.5f, 0.5f, true), Vector3(1.0f, 1.0f, true), Vector2(0.5f, 0.5f), new Texture("res/guis/eg_background.png"), 1)),
		m_guiLogo(new Gui(this, Vector3(0.5f, 0.5f, true), Vector3(0.4f, 0.4f, true), Vector2(0.5f, 0.5f), new Texture("res/guis/equilibrium_games.png"), 1)),
		m_textCopyright(new Text(this, Vector2(0.5f, 0.82f), Vector2(0.5f, 0.5f), "Copyright (C) 2017, Equilibrium Games - All Rights Reserved. This product uses GLFW, Vulkan, and STB Image.", 10.0f, Uis::Get()->m_cafeFrancoise, 0.5f, JustifyCentre)),
		m_starting(true)
	{
		m_textCopyright->SetTextColour(Colour("#ffffff"));

		Events::Get()->AddEvent(new EventTime(1.649f, false, [&]()
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
