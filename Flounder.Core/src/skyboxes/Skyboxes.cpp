#include "Skyboxes.hpp"

namespace Flounder
{
	Skyboxes::Skyboxes() :
		IModule(),
		m_skybox(nullptr),
		m_fog(new Fog(new Colour(), 0.001f, 2.0f, -1.28f, 51.2f))
	{
	}

	Skyboxes::~Skyboxes()
	{
		delete m_skybox;
		delete m_fog;
	}

	void Skyboxes::Update()
	{
		if (m_skybox != nullptr)
		{
			m_skybox->Update();
		}
	}
}
