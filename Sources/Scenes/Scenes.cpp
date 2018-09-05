#include "Scenes.hpp"

#include "Physics/Collider.hpp"

namespace acid
{
	Scenes::Scenes() :
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

		auto &gameObjects = m_scene->GetStructure()->GetAll();

		for (auto it = gameObjects.begin(); it != gameObjects.end();)
		{
			(*it)->Update();

			if ((*it)->IsRemoved())
			{
				it = gameObjects.erase(it);
				continue;
			}

			++it;
		}

		if (m_scene->GetCamera() == nullptr)
		{
			return;
		}

		m_scene->GetCamera()->Update();
	}
}
