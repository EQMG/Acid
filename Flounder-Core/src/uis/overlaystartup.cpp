#include "overlaystartup.hpp"

namespace Flounder
{
	overlaystartup::overlaystartup(uiobject *parent) :
		uiobject(parent, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f)),
		m_guiBackground(new gui(this, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f), new texture("res/guis/eg_background.png"), 1)),
		m_guiLogo(new gui(this, vector2(0.5f, 0.5f), vector2(0.4f, 0.4f), new texture("res/guis/equilibrium_games.png"), 1)),
		m_textCopyright(new text(this, vector2(0.5f, 0.82f), "Copyright (C) 2017, Equilibrium Games - All Rights Reserved. This product uses OpenAL, GLFW, Vulkan, and STB Image.", 1.5f, uis::get()->m_cafeFrancoise, 0.5f, CENTRE)),
		m_starting(true)
	{
		setInScreenCoords(false);

		m_guiBackground->setInScreenCoords(true);
		m_guiLogo->setInScreenCoords(true);
		m_textCopyright->setTextColour(colour(1.0f, 1.0f, 1.0f, 1.0f));

		events::get()->addEvent(new eventtime(1.649f, false, [&]()
		{
			setAlphaDriver(new driverslide(1.0f, 0.0f, 1.0f));
		}));
	}

	overlaystartup::~overlaystartup()
	{
		delete m_guiBackground;
		delete m_guiLogo;
		delete m_textCopyright;
	}

	void overlaystartup::updateObject()
	{
		m_guiBackground->getDimensions()->m_x = Display::get()->getAspectRatio();
		m_guiBackground->setVisible(true);
		m_guiLogo->setVisible(true);
	}
}
