#include "MeshTerrain.hpp"

#include "Worlds/Worlds.hpp"

namespace fl
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

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, Transform *transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_radius(radius),
		m_transform(transform)
	{
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		if (m_radius == 0.0f)
		{
			return Vector3(x, 0.0f, z);
		}

		Vector4 cartesian = Vector4(x, 0.0f, z, 1.0f);
		cartesian = m_transform->GetWorldMatrix().Multiply(cartesian);
		cartesian = Vector3(cartesian).ProjectCubeToSphere(m_radius);
		Vector3 polar = Vector3(cartesian).CartesianToPolar();

		//	polar.m_x = m_radius + (28.0f * Terrains::Get()->GetNoise()->GetValue((m_radius / 30.0f) * cartesian.m_x, (m_radius / 30.0f) * cartesian.m_y, (m_radius / 30.0f) * cartesian.m_z));
		polar.m_x = Worlds::Get()->GetTerrainRadius(m_radius, polar.m_y, polar.m_z);

		return polar.PolarToCartesian();
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		/*	Vector3 polar = Vector3::CartesianToPolar(position);

			float squareSize = 0.1f;
			Vector3 positionL = GetPosition(polar.m_y - squareSize, polar.m_z);
			Vector3 positionR = GetPosition(polar.m_y + squareSize, polar.m_z);
			Vector3 positionD = GetPosition(polar.m_y, polar.m_z - squareSize);
			Vector3 positionU = GetPosition(polar.m_y, polar.m_z + squareSize);

			Vector3 normal = Vector3();
			Vector3::Cross(positionL - positionR, positionD - positionU, &normal);
			normal.Normalize();
			return normal;*/

		//	Vector3 normal = Vector3();
		//	Vector3::Cross(positionL - positionR, positionR - positionD, &normal);
		//	normal.Normalize();
		//	return normal;

		//	return Vector3::ProjectCubeToSphere(m_radius, position);
		return Vector3::ZERO;
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		Vector3 polar = position.CartesianToPolar();
		float value = (polar.m_x - m_radius + COLOUR_AMPLITUDE) / (COLOUR_AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - COLOUR_HALF_SPREAD) * (1.0f / COLOUR_SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / COLOUR_PART));
		float blend = (value - (firstBiome * COLOUR_PART)) / COLOUR_PART;
		Colour colour = COLOUR_BIOMES.at(firstBiome).Interpolate(COLOUR_BIOMES.at(firstBiome + 1), blend);
		return colour;
	}
}
