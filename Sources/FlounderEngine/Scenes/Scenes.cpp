#include "Scenes.hpp"

namespace Flounder
{
	Scenes::Scenes() :
		IModule(),
		m_structure(nullptr)
	{
	}

	Scenes::~Scenes()
	{
		delete m_structure;
	}

	void Scenes::Update()
	{
		if (m_structure == nullptr)
		{
			return;
		}

		for (auto object : *m_structure->GetAll())
		{
			object->Update();
		}
	}
}
