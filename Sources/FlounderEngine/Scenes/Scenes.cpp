#include "Scenes.hpp"

namespace Flounder
{
	Scenes::Scenes() :
		IModule(),
		m_scene(nullptr)
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

		m_scene->Update();

		if (m_scene->GetStructure() == nullptr)
		{
			return;
		}

		for (auto object : *m_scene->GetStructure()->GetAll())
		{
			object->Update();
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
		if (m_scene != scene)
		{
			delete m_scene;
		}

		m_scene = scene;
		m_scene->Start();
	}
}
