#include "terrains.hpp"

namespace Flounder
{
	Terrains::Terrains() :
		IModule(),
		m_terrains(new std::vector<Terrain*>())
	{
		m_terrains->push_back(new Terrain(
			Vector3(), Vector3(), 420
		));

		for (int j = -1; j <= 1; j++)
		{
			for (int w = -1; w <= 1; w++)
			{
				if (!(j == 0 && w == 0))
				{
					m_terrains->push_back(new Terrain(
						Vector3(2.0f * static_cast<float>(j) * Terrain::SIDE_LENGTH, 0.0f, 2.0f * static_cast<float>(w) * Terrain::SIDE_LENGTH), Vector3(), 420
					));
				}
			}
		}
	}

	Terrains::~Terrains()
	{
		delete m_terrains;
	}

	void Terrains::Update()
	{
		for (Terrain *object : *m_terrains)
		{
			bool inRange = true;
			bool current = true;
			// if !inRange ? delete
			// if current && !wasCurrent ? addChildren
			object->Update();
		}
	}
}
