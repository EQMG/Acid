#include "Planet.hpp"

#include "../Maths/Maths.hpp"
#include "VoxelRender.hpp"

namespace Flounder
{
	Planet::Planet(const int &sideLength) :
		m_chunks(new std::vector<Chunk *>(sideLength * sideLength)),
		m_sideLength(sideLength),
		m_generate(true)
	{
	}

	Planet::~Planet()
	{
		for (auto chunk : *m_chunks)
		{
			delete chunk;
		}

		delete m_chunks;
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

	Vector3 Planet::GetSideDirection(const PlanetSide &side)
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

	PlanetSide Planet::GetSide(const Vector3 &position, const int &sideLength)
	{
		return SideLeft;
	}

	void Planet::Generate()
	{
		// Chunks.
		new GameObject("PlanetCentre", Transform(Vector3(), Vector3(), 1.0f));

		for (int j = 0; j < m_sideLength; j++)
		{
			for (int k = 0; k < m_sideLength; k++)
			{
				for (int l = 0; l < m_sideLength; l++)
				{
					Vector3 position = *Chunk::CHUNK_SIZE * Vector3(j, 0, k);
				//	PlanetSide side = GetSide(position, m_sideLength);

					GameObject *chunk = new GameObject(Transform(*Chunk::CHUNK_SIZE * Vector3(j, l, k)));
					chunk->SetName("Chunk" + GetName() + "," + std::to_string(j) + "," + std::to_string(l) + "," + std::to_string(k));
					chunk->AddComponent(new Chunk(MeshGreedy, true));
					chunk->AddComponent(new Mesh());
					chunk->AddComponent(new VoxelRender());
					//	chunk->AddComponent(new ShadowRender());

					m_chunks->at(j * k) = chunk->GetComponent<Chunk>();
				}
			}
		}
	}
}
