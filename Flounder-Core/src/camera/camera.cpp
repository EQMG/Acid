#include "Camera.hpp"

namespace Flounder
{
	Camera::Camera() :
		IModule(),
		m_camera(nullptr),
		m_player(nullptr)
	{
	}

	Camera::~Camera()
	{
		delete m_camera;
		delete m_player;
	}

	void Camera::Update()
	{
		if (m_player != nullptr)
		{
			m_player->Update();
		}

		if (m_camera != nullptr)
		{
			m_camera->Update(m_player);
		}
	}
}
