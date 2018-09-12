#include "Scenes.hpp"

#include "Physics/Collider.hpp"

namespace acid
{
	Scenes::Scenes() :
		m_scene(nullptr),
		m_componentRegister(ComponentRegister())
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

		m_scene->GetPhysics()->Update();
		m_scene->Update();

		if (m_scene->GetStructure() == nullptr)
		{
			return;
		}

		auto gameObjects = m_scene->GetStructure()->QueryAll();

		for (auto &gameObject : gameObjects)
		{
			gameObject->Update();
		}

		if (m_scene->GetCamera() == nullptr)
		{
			return;
		}

		m_scene->GetCamera()->Update();
	}
}
