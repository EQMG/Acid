#include "guis.h"

namespace flounder
{
	guis::guis()
		: imodule()
	{
		m_guiMaster = NULL;
		m_selector = new guiselector();
		m_container = new screenobjectempty(NULL, vector2(0.5f, 0.5f), vector2(1.0f, 1.0f), false);
		m_objects = new std::vector<screenobject*>();
	}

	guis::~guis()
	{
		delete m_guiMaster;
		delete m_selector;
		delete m_container;
		delete m_objects;
	}

	void guis::load(iguimanager *guiMaster)
	{
		m_guiMaster = guiMaster;
	}

	void guis::init()
	{
	}

	void guis::update()
	{
		if (m_guiMaster != NULL)
		{
			m_selector->update(m_guiMaster->isGamePaused());
			m_guiMaster->update();
		}

		m_objects->clear();
		m_container->update();
		m_container->getAll(m_objects);
	}
}
