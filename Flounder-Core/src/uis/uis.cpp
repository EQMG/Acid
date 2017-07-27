#include "uis.h"

namespace flounder
{
	uis::uis() :
		imodule()
	{
		m_uiManager = NULL;
		m_selector = new uiselector();
		m_container = new containerscreen(NULL, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f), false);
		m_objects = new std::vector<uiobject*>();

		arial = new fonttype("res/fonts/arial.png", "res/fonts/arial.fnt");
		berlinSans = new fonttype("res/fonts/berlinSans.png", "res/fonts/berlinSans.fnt");
		cafeFrancoise = new fonttype("res/fonts/cafefrancoise.png", "res/fonts/cafefrancoise.fnt");
		candara = new fonttype("res/fonts/candara.png", "res/fonts/candara.fnt");
		segoe = new fonttype("res/fonts/segoe.png", "res/fonts/segoe.fnt");
	}

	uis::~uis()
	{
		delete m_uiManager;
		delete m_selector;
		delete m_container;
		delete m_objects;

		delete arial;
		delete berlinSans;
		delete cafeFrancoise;
		delete candara;
		delete segoe;
	}

	void uis::load(iuimanager *uiManager)
	{
		m_uiManager = uiManager;
	}

	void uis::init()
	{
	}

	void uis::update()
	{
		if (m_uiManager != NULL)
		{
			m_selector->update(m_uiManager->isGamePaused());
			m_uiManager->update();
		}

		m_objects->clear();
		m_container->update();
		m_container->getAll(m_objects);
	}
}
