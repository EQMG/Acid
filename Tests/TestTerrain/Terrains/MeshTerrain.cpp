#include "MeshTerrain.hpp"

#include <Worlds/Worlds.hpp>

namespace test
{
	const int MeshTerrain::SIDE_LENGTH = 200;
	const std::vector<float> MeshTerrain::SQUARE_SIZES = {
		4.0f, 8.0f, 25.0f, 50.0f, 100.0f
	};
	const std::vector<float> MeshTerrain::TEXTURE_SCALES = {
		10.0f, 5.0f, 2.0f, 1.0f, 0.5f
	};

	const std::array<Colour, 4> COLOUR_BIOMES = {
		Colour("#6e3529"), Colour("#934838"), Colour("#9e402c"), Colour("#656565")
	};
	const float COLOUR_SPREAD = 0.76f;
	const float COLOUR_HALF_SPREAD = COLOUR_SPREAD / 2.0f;
	const float COLOUR_AMPLITUDE = 20.0f;
	const float COLOUR_PART = 1.0f / (COLOUR_BIOMES.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, Transform *transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_transform(transform)
	{
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		Vector3 position = Vector3(x, 0.0f, z);
		position.m_y = GetHeight(position.m_x + m_transform->GetPosition().m_x, position.m_z + m_transform->GetPosition().m_z);
		return position;
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		return GetNormal(position.m_x + m_transform->GetPosition().m_x, position.m_z + m_transform->GetPosition().m_z);
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		float value = (position.m_y + COLOUR_AMPLITUDE) / (COLOUR_AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - COLOUR_HALF_SPREAD) * (1.0f / COLOUR_SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / COLOUR_PART));
		float blend = (value - (firstBiome * COLOUR_PART)) / COLOUR_PART;
		Colour colour = COLOUR_BIOMES.at(firstBiome).Interpolate(COLOUR_BIOMES.at(firstBiome + 1), blend);
		return colour;
	}

	float MeshTerrain::GetHeight(const float &x, const float &z)
	{
		float height1 = Worlds::Get()->GetNoiseTerrain().GetNoise(x, z);
		height1 = (height1 * 60.0f) + 15.0f;
		//	float length = std::sqrt((x * x) + (z * z));

		//	if (length >= 700.0f)
		//	{
		//		height1 = height1 - (0.2f * (length - 700.0f));
		//	}

		return height1;
	}

	Vector3 MeshTerrain::GetNormal(const float &x, const float &z)
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
}
