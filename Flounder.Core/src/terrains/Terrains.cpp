#include "terrains.hpp"

#include "../worlds/Worlds.hpp"

namespace Flounder
{
	Terrains::Terrains() :
		IModule(),
		m_terrains(new std::vector<Terrain*>()),
		m_noiseTerrain(NoiseFast(5413212))
	{
		m_noiseTerrain.SetNoiseType(NoiseFast::PerlinFractal);
		m_noiseTerrain.SetFrequency(0.00175f);
		m_noiseTerrain.SetFractalType(NoiseFast::Fbm);
		m_noiseTerrain.SetFractalOctaves(5);
		m_noiseTerrain.SetFractalLacunarity(2.0f);
		m_noiseTerrain.SetFractalGain(0.55f);
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
		const float worldNoiseHeight = 156.0f;
		const float worldNoiseOffset = 8.0f;

		float height = m_noiseTerrain.GetNoise(x, z);
		height *= worldNoiseHeight;
		height += worldNoiseOffset;
		return height;
	}

	float Terrains::GetSlope(const float &x, const float &z)
	{
		const float squareSize = 0.5f;
		const float heightL = GetHeight(x - squareSize, z);
		const float heightR = GetHeight(x + squareSize, z);
		const float heightD = GetHeight(x, z - squareSize);
		const float heightU = GetHeight(x, z + squareSize);

		Vector3 normal = Vector3(heightL - heightR, 0.5f, heightD - heightU);
		normal.Normalize();
		return 1.0f - normal.m_y;
	}

	Vector3 Terrains::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, GetHeight(x, z), z);
	}
}
