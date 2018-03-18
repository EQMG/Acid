#include "Planet.hpp"

#include "../Maths/Maths.hpp"
#include "VoxelRender.hpp"

namespace Flounder
{
	Planet::Planet(const int &radius) :
		m_sides(new std::map<PlanetSide, std::vector<Chunk *>>()),
		m_radius(radius),
		m_generate(true)
	{
	}

	Planet::~Planet()
	{
		for (auto side : *m_sides)
		{
			for (auto chunk : side.second)
			{
				delete chunk;
			}
		}

		delete m_sides;
	}

	void Planet::Update()
	{
		if (m_generate)
		{
			Generate();
			m_generate = false;
		}
	}

	void Planet::Load(LoadedValue *value)
	{
	}

	void Planet::Write(LoadedValue *value)
	{
	}

	Vector3 Planet::GetDirection(const PlanetSide &side)
	{
		switch (side)
		{
		case SideFront:
			return Vector3::FRONT;
		case SideBack:
			return Vector3::BACK;
		case SideUp:
			return Vector3::UP;
		case SideDown:
			return Vector3::DOWN;
		case SideLeft:
			return Vector3::LEFT;
		case SideRight:
			return Vector3::RIGHT;
		default:
			return nullptr;
		}
	}

	void Planet::Generate()
	{
		// Calculates the side length of the cube.
		int sideLength = ((int) std::pow (2, m_radius)) / 8;
		printf("Planet radius: %i, side length: %i\n", m_radius, sideLength);

		// Chunks.
		new GameObject("Testing", Transform(Vector3(), Vector3(), 1.0f));

		for (int s = 0; s < 6; s++)
		{
			PlanetSide side = (PlanetSide) s;
			std::vector<Chunk *> chunks = std::vector<Chunk *>(sideLength * sideLength);

			for (int j = 0; j < sideLength; j++)
			{
				for (int k = 0; k < sideLength; k++)
				{
					GameObject *chunk = new GameObject(Transform(*Chunk::CHUNK_SIZE * Vector3(j, 0, k)));
					chunk->SetName("Chunk" + GetName() + "," + std::to_string(s) + "," + std::to_string(j) + "," + std::to_string(k));
					chunk->AddComponent(new Chunk(MeshGreedy, true));
					chunk->AddComponent(new Mesh());
					//	chunk->AddComponent(new Material());
					chunk->AddComponent(new VoxelRender());
					//	chunk->AddComponent(new ShadowRender());

					chunks.at(j * k) = chunk->GetComponent<Chunk>();
				}
			}

			m_sides->insert(std::make_pair(side, chunks));
		}
	}
}
