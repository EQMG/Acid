#include "uis.hpp"

namespace flounder
{
	uis::uis() :
		imodule()
	{
		m_managerUis = nullptr;
		m_selector = new uiselector();
		m_container = new containerscreen(nullptr, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f), false);
		m_objects = new std::vector<uiobject*>();

		arial = new fonttype("res/fonts/arial.png", "res/fonts/arial.fnt");
		berlinSans = new fonttype("res/fonts/berlinSans.png", "res/fonts/berlinSans.fnt");
		cafeFrancoise = new fonttype("res/fonts/cafefrancoise.png", "res/fonts/cafefrancoise.fnt");
		candara = new fonttype("res/fonts/candara.png", "res/fonts/candara.fnt");
		segoe = new fonttype("res/fonts/segoe.png", "res/fonts/segoe.fnt");
	}

	uis::~uis()
	{
		delete m_managerUis;
		delete m_selector;
		delete m_container;
		delete m_objects;

		delete arial;
		delete berlinSans;
		delete cafeFrancoise;
		delete candara;
		delete segoe;
	}

	void uis::update()
	{
		if (m_managerUis != nullptr)
		{
			m_selector->update(m_managerUis->isGamePaused());
			m_managerUis->update();
		}

		m_objects->clear();
		m_container->update();
		m_container->getAll(m_objects);
	}
}
