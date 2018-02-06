#include "Entities.hpp"

namespace Flounder
{
	Entities::Entities() :
		IModule(),
		m_structure(nullptr)
	{
	}

	Entities::~Entities()
	{
		delete m_structure;
		//	delete m_entities;
	}

	void Entities::Update()
	{
		if (m_structure == nullptr)
		{
			return;
		}

		for (auto entity : *m_structure->GetAll())
		{
			entity->Update();
		}
	}
}
