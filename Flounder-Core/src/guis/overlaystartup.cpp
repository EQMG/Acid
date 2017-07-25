#include "overlaystartup.h"
#include "guis.h"

namespace flounder
{
	overlaystartup::overlaystartup(screenobject *parent) :
		screenobject(parent, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f))
	{
		setInScreenCoords(false);

		m_background = new guiobject(this, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f), texture::newTexture()->setFile("res/guis/eg_background.png")->create(), 1);
		m_background->setInScreenCoords(true);

		m_logo = new guiobject(this, vector2(0.5f, 0.5f), vector2(0.4f, 0.4f), texture::newTexture()->setFile("res/guis/equilibrium_games.png")->create(), 1);
		m_logo->setInScreenCoords(true);

		//	m_text = new textobject(this, vector2(0.5f, 0.82f), " Copyright (C) 2017, Equilibrium Games - All Rights Reserved. This product uses GLFW, GLEW, STB Image, and Steamworks, along with technologies from The Khronos Group Inc.", 1.32f, fonts::get()->CAFE_FRANCOISE, 0.6f, CENTRE);
		//	m_text->setColour(colour(1.0f, 1.0f, 1.0f));

		m_starting = true;

		events::get()->addEvent(new eventtime(5.0f, false, [&]()
	    {
			setAlphaDriver(new driverslide(1.0f, 0.0f, 1.0f));
	    }));
	}

	overlaystartup::~overlaystartup()
	{
		delete m_background;
		delete m_logo;
		//	delete m_text;
	}

	void overlaystartup::updateObject()
	{
		m_background->getDimensions()->m_x = display::get()->getAspectRatio();
		m_background->setVisible(true);
		m_logo->setVisible(true);
	}
}
