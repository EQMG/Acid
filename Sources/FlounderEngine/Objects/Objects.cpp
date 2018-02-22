#include "Objects.hpp"

namespace Flounder
{
	Objects::Objects() :
		IModule(),
		m_structure(nullptr)
	{
	}

	Objects::~Objects()
	{
		delete m_structure;
	}

	void Objects::Update()
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
