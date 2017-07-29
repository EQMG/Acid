#include "overlaystartup.h"

namespace flounder
{
	overlaystartup::overlaystartup(uiobject *parent) :
		uiobject(parent, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f))
	{
		setInScreenCoords(false);

		m_guiBackground = new gui(this, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f), texture::newTexture()->setFile("res/guis/eg_background.png")->create(), 1);
		m_guiBackground->setInScreenCoords(true);

		m_guiLogo = new gui(this, vector2(0.5f, 0.5f), vector2(0.4f, 0.4f), texture::newTexture()->setFile("res/guis/equilibrium_games.png")->create(), 1);
		m_guiLogo->setInScreenCoords(true);
		
		m_textCopyright = new text(this, vector2(0.5f, 0.82f), "Copyright (C) 2017, Equilibrium Games - All Rights Reserved. This product uses GLFW, GLEW, and STB Image.", 1.5f, uis::get()->cafeFrancoise, 0.5f, CENTRE);
		m_textCopyright->setTextColour(colour(1.0f, 1.0f, 1.0f));

		m_starting = true;

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
		m_guiBackground->getDimensions()->m_x = display::get()->getAspectRatio();
		m_guiBackground->setVisible(true);
		m_guiLogo->setVisible(true);
	}
}
