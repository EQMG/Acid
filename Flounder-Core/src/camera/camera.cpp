#include "camera.h"

namespace flounder
{
	camera::camera() :
		imodule()
	{
		m_camera = nullptr;
		m_player = nullptr;
	}

	camera::~camera()
	{
		delete m_camera;
		delete m_player;
	}

	void camera::update()
	{
		if (m_player != nullptr)
		{
			m_player->update();
		}

		if (m_camera != nullptr)
		{
			m_camera->update(*m_player);
		}
	}
}
