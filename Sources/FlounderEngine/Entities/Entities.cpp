#include "Entities.hpp"

#include "../Space/StructureBasic.hpp"

namespace Flounder
{
	Entities::Entities() :
		IModule(),
		m_structure(new StructureBasic<Entity*>())
	//	m_entities(new std::vector<Entity*>())
	{
	}

	Entities::~Entities()
	{
		delete m_structure;
	//	delete m_entities;
	}

	void Entities::Update()
	{
		for (auto entity : *m_structure->GetAll())
		{
			entity->Update();
		}
	}
}
