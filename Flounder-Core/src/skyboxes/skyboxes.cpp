#include "skyboxes.hpp"

namespace Flounder
{
	skyboxes::skyboxes() :
		IModule(),
		m_skybox(nullptr),
		m_fog(new fog(new colour(), 0.001f, 2.0f, -1.28f, 51.2f))
	{
	}

	skyboxes::~skyboxes()
	{
		delete m_skybox;
		delete m_fog;
	}

	void skyboxes::Update()
	{
		if (m_skybox != nullptr)
		{
			m_skybox->update();
		}
	}
}
