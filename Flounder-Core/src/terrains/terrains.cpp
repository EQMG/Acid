#include "terrains.h"

namespace flounder
{
	terrains::terrains() :
		imodule()
	{
		m_terrain = NULL;
	}

	terrains::~terrains()
	{
		delete m_terrain;
	}

	void terrains::update()
	{
		if (m_terrain != NULL)
		{
			m_terrain->update();
		}
	}
}
