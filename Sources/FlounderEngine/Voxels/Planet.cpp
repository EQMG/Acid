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

	Vector3 Planet::GetSideRotation(const PlanetSide &side)
	{
		switch (side)
		{
		case SideFront:
			return Vector3(0.0f, 270.0f, 0.0f);
		case SideBack:
			return Vector3(0.0f, 90.0f, 0.0f);
		case SideUp:
			return Vector3(0.0f, 0.0f, 90.0f);
		case SideDown:
			return Vector3(0.0f, 0.0f, 270.0f);
		case SideLeft:
			return Vector3(0.0f, 180.0f, 0.0f);
		case SideRight:
			return Vector3(0.0f, 0.0f, 0.0f);
		default:
			return nullptr;
		}
	}

	Vector3 Planet::EvaluateVertex(const Vector3 &position, const int &size)
	{
		float xRad = -45.0f + 90.0f * position.m_x / ((float)size);
		float yRad = -45.0f + 90.0f * position.m_y / ((float)size);
		float zRad = -45.0f + 90.0f * position.m_z / ((float)size);

		xRad = ((PI * 2.0f) / 360.0f) * xRad;
		yRad = ((PI * 2.0f) / 360.0f) * yRad;
		zRad = ((PI * 2.0f) / 360.0f) * zRad;

		Vector3 result = Vector3(std::sin(xRad) / std::cos(xRad), std::sin(yRad) / std::cos(yRad), std::sin(zRad) / std::cos(zRad));
		result.Normalize();
		return result;
	}

	void Planet::Generate()
	{
		// Calculates the side length of the cube.
		int sideLength = ((int) std::pow (2, m_radius)) / 8;
		int size = (int) std::floor(std::pow(2.0f, m_radius));
		int rMin = (int) std::floor((2.0f / PI - 1.0f / 8.f) * size);
		printf("Planet radius: %i, side length: %i, size: %i, rMin: %i\n", m_radius, sideLength, size, rMin);

		// Chunks.
		new GameObject("PlanetCentre", Transform(Vector3(), Vector3(), 1.0f));

		int s = 0;
		// for (int s = 0; s < 6; s++)
		{
			PlanetSide side = (PlanetSide) s;
			std::vector<Chunk *> chunks = std::vector<Chunk *>(sideLength * sideLength);

			for (int j = 0; j < sideLength; j++)
			{
				for (int k = 0; k < sideLength; k++)
				{
				//	Vector3 local = (*Chunk::CHUNK_SIZE / 2.0f) + (*Chunk::CHUNK_SIZE * Vector3(j, 0.0f, k));
				//	Vector3 localUp = EvaluateVertex((int)local.m_x, (int)local.m_z, size);
				//	printf("%i: Up: %s\n", side, localUp.ToString().c_str());

					Vector3 rotation = Vector3(); // GetSideRotation(side);
					Vector3 position = *Chunk::CHUNK_SIZE * Vector3(j, 0, k);
				//	Vector3::Rotate(position, rotation, &position);
				//	position = position + (sideLength * *Chunk::CHUNK_SIZE * GetSideDirection(side));
				//	position = position - ((*Chunk::CHUNK_SIZE * sideLength) / 2.0f);

					GameObject *chunk = new GameObject(Transform(position, rotation));
					chunk->SetName("Chunk" + GetName() + "," + std::to_string(s) + "," + std::to_string(j) + "," + std::to_string(k));
					chunk->AddComponent(new Chunk(MeshGreedy, true));
					chunk->AddComponent(new Mesh());
					chunk->AddComponent(new VoxelRender());
					//	chunk->AddComponent(new ShadowRender());

					chunks.at(j * k) = chunk->GetComponent<Chunk>();
				}
			}

			m_sides->insert(std::make_pair(side, chunks));
		}
	}
}
