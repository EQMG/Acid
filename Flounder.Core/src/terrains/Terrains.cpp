#include "terrains.hpp"

#include "../worlds/Worlds.hpp"

namespace Flounder
{
	Terrains::Terrains() :
		IModule(),
		m_terrains(new std::vector<Terrain*>()),
		m_noiseTerrain(NoiseFast(876759995)),
		m_noiseRivers(NoiseFast(1045211682))
	{
		m_noiseTerrain.SetNoiseType(NoiseFast::SimplexFractal);
		m_noiseTerrain.SetFrequency(0.007f);
		m_noiseTerrain.SetFractalType(NoiseFast::Fbm);
		m_noiseTerrain.SetFractalOctaves(4);
		m_noiseTerrain.SetFractalLacunarity(2.0f);
		m_noiseTerrain.SetFractalGain(0.4f);

		m_noiseRivers.SetNoiseType(NoiseFast::SimplexFractal);
		m_noiseRivers.SetFrequency(0.01f);
		m_noiseRivers.SetFractalType(NoiseFast::Billow);
		m_noiseRivers.SetFractalOctaves(1);
		m_noiseRivers.SetFractalLacunarity(2.0f);
		m_noiseRivers.SetFractalGain(0.5f);
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
		const float worldNoiseHeight = 35.0f;
		const float worldNoiseOffset = 7.0f;

		//	float river = m_noiseRivers.GetNoise(x, z);
		//	river = (river <= 0.0f) ? river : 1.0f;
		float height = m_noiseTerrain.GetNoise(x, z);
		height = (height < 0.0f ? -1.0f : 1.0f) * pow(height, 2.0f);
		//	height *= river;
		height *= worldNoiseHeight;
		height += worldNoiseOffset;
		return height;
	}

	Vector3 Terrains::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, GetHeight(x, z), z);
	}
}
