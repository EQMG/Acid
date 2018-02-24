#include "Terrains.hpp"

#include "../Worlds/Worlds.hpp"

namespace Flounder
{
	Terrains::Terrains() :
		IModule(),
		m_noise1(NoiseFast(8420152))
	{
		m_noise1.SetNoiseType(NoiseFast::PerlinFractal);
		m_noise1.SetFrequency(0.003f);
		m_noise1.SetInterp(NoiseFast::Quintic);
		m_noise1.SetFractalType(NoiseFast::Fbm);
		m_noise1.SetFractalOctaves(5);
		m_noise1.SetFractalLacunarity(2.0);
		m_noise1.SetFractalGain(0.55f);
	}

	Terrains::~Terrains()
	{
	}

	void Terrains::Update()
	{
	}

	float Terrains::GetHeight(const float &x, const float &z)
	{
		float height1 = m_noise1.GetNoise(x, z);
		height1 = (height1 * 60.0f) + 15.0f;
		float length = std::sqrt((x * x) + (z * z));

		if (length >= 700.0f)
		{
			height1 = height1 - (0.2f * (length - 700.0f));
		}

		return height1;
	}

	Vector3 Terrains::GetNormal(const float &x, const float &z)
	{
		const float squareSize = 0.1f;
		const float heightL = GetHeight(x - squareSize, z);
		const float heightR = GetHeight(x + squareSize, z);
		const float heightD = GetHeight(x, z - squareSize);
		const float heightU = GetHeight(x, z + squareSize);

		Vector3 normal = Vector3(heightL - heightR, squareSize, heightD - heightU);
		// float slope = 1.0f - normal.m_y;
		normal.Normalize();
		return normal;
	}

	Vector3 Terrains::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, GetHeight(x, z), z);
	}
}
