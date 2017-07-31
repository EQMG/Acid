#include "skyboxes.h"

namespace flounder
{
	skyboxes::skyboxes() :
		imodule()
	{
		m_skybox = NULL;
		m_fog = new fog(new colour(), 0.001f, 2.0f, -1.28f, 51.2f);
	}

	skyboxes::~skyboxes()
	{
		delete m_skybox;
		delete m_fog;
	}

	void skyboxes::update()
	{
		if (m_skybox != NULL)
		{
			m_skybox->update();
		}
	}
}
