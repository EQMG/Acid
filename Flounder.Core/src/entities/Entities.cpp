#include "Entities.hpp"

#include "../space/StructureBasic.hpp"

namespace Flounder
{
	Entities::Entities() :
		IModule()
	//	m_structure(new StructureBasic<Entity*>())
	{
	}

	Entities::~Entities()
	{
		delete m_structure;
	}

	void Entities::Update()
	{
	//	for (auto entity : *m_structure->GetAll())
	//	{
	//		entity->Update();
	//	}
	}
}
