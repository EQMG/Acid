#include "Skyboxes.hpp"

namespace Flounder
{
	Skyboxes::Skyboxes() :
		IModule(),
		m_skybox(nullptr),
		m_fog(new Fog(new Colour(), 0.001f, 2.0f, -0.1f, 0.3f)),
		m_skyColour(new Colour("#3399ff"))
	{
	}

	Skyboxes::~Skyboxes()
	{
		delete m_skybox;
		delete m_fog;
		delete m_skyColour;
	}

	void Skyboxes::Update()
	{
		if (m_skybox != nullptr)
		{
			m_skybox->Update();
		}
	}
}
