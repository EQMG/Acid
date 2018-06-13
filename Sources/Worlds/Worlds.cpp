#include "Worlds.hpp"

namespace fl
{
	Worlds::Worlds() :
		IModule(),
		m_world(nullptr)
	{
	}

	Worlds::~Worlds()
	{
	}

	void Worlds::Update()
	{
		if (m_world == nullptr)
		{
			return;
		}

		m_world->Update();
	}

	void Worlds::SetWorld(IWorld *world)
	{
		if (m_world != world)
		{
			delete m_world;
		}

		m_world = world;
	}
}
