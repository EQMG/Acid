#include "terrains.hpp"

#include "../worlds/Worlds.hpp"

namespace Flounder
{
	Terrains::Terrains() :
		IModule(),
		m_terrains(new std::vector<Terrain*>()),
		m_noiseTerrain(NoiseFast(1963))
	{
		m_noiseTerrain.SetNoiseType(NoiseFast::PerlinFractal);
		m_noiseTerrain.SetFrequency(0.0015f);
		m_noiseTerrain.SetInterp(NoiseFast::Quintic);
		m_noiseTerrain.SetFractalType(NoiseFast::Fbm);
		m_noiseTerrain.SetFractalOctaves(5);
		m_noiseTerrain.SetFractalLacunarity(2.0f);
		m_noiseTerrain.SetFractalGain(0.53f);
	}

	Terrains::~Terrains()
	{
		for (auto object : *m_terrains)
		{
			delete object;
		}

		delete m_terrains;
	}

	void Terrains::Update()
	{
		for (auto object : *m_terrains)
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
		const float worldNoiseHeight = 100.0f;
		const float worldNoiseOffset = 20.0f;

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
