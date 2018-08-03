#include "Scenes.hpp"

#include "Physics/Collider.hpp"

namespace acid
{
	Scenes::Scenes() :
		IModule(),
		m_scene(nullptr),
		m_componentRegister(ComponentRegister())
	{
	}

	Scenes::~Scenes()
	{
		delete m_scene;
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

		m_scene->GetPhysics()->Update();
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

	void Scenes::SetScene(IScene *scene)
	{
		delete m_scene;
		m_scene = scene;
	}
}
