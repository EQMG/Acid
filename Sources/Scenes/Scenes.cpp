#include "Scenes.hpp"

namespace fl
{
	Scenes::Scenes() :
		IModule(),
		m_componentRegister(new ComponentRegister()),
		m_scene(nullptr)
	{
	}

	Scenes::~Scenes()
	{
		delete m_scene;
		delete m_componentRegister;
	}

	void Scenes::Update()
	{
		if (m_scene == nullptr)
		{
			return;
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
		m_scene->GetUiManager()->Update();
	}

	void Scenes::SetScene(Scene *scene)
	{
		//	if (m_scene != scene)
		//	{
		//		delete m_scene;
		//	}

		m_scene = scene;
		m_scene->Start();
	}
}
