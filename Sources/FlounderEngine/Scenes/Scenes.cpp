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
	}
}
