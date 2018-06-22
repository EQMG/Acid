#include "Scenes.hpp"

namespace fl
{
	Scenes::Scenes() :
		IModule(),
		m_scene(nullptr),
		m_componentRegister(ComponentRegister())
	{
	}

	Scenes::~Scenes()
	{
	}

	void Scenes::Update()
	{
		if (m_scene == nullptr)
		{
			return;
		}

		if (!m_scene->IsStarted())
		{
			m_scene->Start();
			m_scene->SetStarted(true);
		}

		m_scene->Update();

		if (m_scene->GetStructure() == nullptr)
		{
			return;
		}

		auto gameObjects = m_scene->GetStructure()->GetAll();

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		{
			if ((*it) == nullptr)
			{
				continue;
			}

			(*it)->Update();
		}

		if (m_scene->GetCamera() == nullptr)
		{
			return;
		}

		m_scene->GetCamera()->Update();
	}
}
