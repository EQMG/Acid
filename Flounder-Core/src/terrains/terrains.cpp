#include "terrains.hpp"

namespace flounder
{
	terrains::terrains() :
		imodule(),
		m_terrains(new std::vector<terrain*>())
	{
		m_terrains->push_back(new terrain(
			vector3(), vector3(), 420
		));

		for (int j = -1; j <= 1; j++)
		{
			for (int w = -1; w <= 1; w++)
			{
				if (!(j == 0 && w == 0))
				{
					m_terrains->push_back(new terrain(
						vector3(2.0f * static_cast<float>(j) * terrain::SIDE_LENGTH, 0.0f, 2.0f * static_cast<float>(w) * terrain::SIDE_LENGTH), vector3(), 420
					));
				}
			}
		}
	}

	terrains::~terrains()
	{
		delete m_terrains;
	}

	void terrains::update()
	{
		for (terrain *object : *m_terrains)
		{
			bool inRange = true;
			bool current = true;
			// if !inRange ? delete
			// if current && !wasCurrent ? addChildren
			object->update();
		}
	}
}
