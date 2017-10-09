#include "terrains.hpp"

#include "../worlds/Worlds.hpp"

namespace Flounder
{
	Terrains::Terrains() :
		IModule(),
		m_terrains(new std::vector<Terrain*>())
	{
		/*for (int j = -1; j <= 1; j++)
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
		}*/

		m_terrains->push_back(new Terrain(Vector3(), Vector3(), 420));
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

	float Terrains::GetHeight(const float &x, const float &z)
	{
		const float worldNoiseSpread = 1.5f;
		const float worldNoiseFrequency = 8.0f;
		const float worldNoiseHeight = 35.0f;
		const float worldNoiseOffset = 0.0f;

		float height = Worlds::Get()->GetNoise()->GetNoise(x / worldNoiseSpread, z / worldNoiseSpread, worldNoiseFrequency);
		height *= worldNoiseHeight;
		height += worldNoiseOffset;
		return height;
	}

	Vector3 Terrains::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, GetHeight(x, z), z);
	}
}
