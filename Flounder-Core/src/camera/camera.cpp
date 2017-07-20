#include "camera.h"

namespace flounder 
{
	camera::camera() :
		imodule()
	{
		m_camera = NULL;
		m_player = NULL;
	}

	camera::~camera()
	{
		delete m_camera;
		delete m_player;
	}

	void camera::load(icamera *camera, iplayer *player)
	{
		m_camera = camera;
		m_player = player;
	}

	void camera::init()
	{
	}

	void camera::update()
	{
		if (m_player != NULL)
		{
			m_player->update();
		}

		if (m_camera != NULL)
		{
			m_camera->update(*m_player);
		}
	}
}
