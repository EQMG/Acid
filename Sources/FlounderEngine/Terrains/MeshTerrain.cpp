#include "MeshTerrain.hpp"

#include "../Maths/Maths.hpp"
#include "../Maths/Colour.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	const std::array<Colour, 4> BIOME_COLOURS = {
		Colour("#6e3529"), Colour("#934838"), Colour("#9e402c"), Colour("#656565")
	};
	const float SPREAD = 0.76f;
	const float HALF_SPREAD = SPREAD / 2.0f;
	const float AMPLITUDE = 20.0f;
	const float PART = 1.0f / (BIOME_COLOURS.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const float &radius, Transform *transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_radius(radius),
		m_transform(transform),
		m_worldMatrix(new Matrix4())
	{
		m_worldMatrix = m_transform->GetWorldMatrix(m_worldMatrix);
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
		delete m_worldMatrix;
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		Vector4 cartesian = Vector4(x, 0.0f, z, 1.0f);
		Matrix4::Multiply(*m_worldMatrix, cartesian, &cartesian);
		cartesian.Set(Vector3::ProjectCubeToSphere(m_radius, cartesian));
		Vector3 polar = Vector3::CartesianToPolar(cartesian);

		polar.m_x = Terrains::Get()->GetRadius(m_radius, polar.m_y, polar.m_z);

		return Vector3::PolarToCartesian(polar);
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
	//	Vector4 normal = Vector4(Terrains::Get()->GetNormal(position.m_x + m_transform->GetPosition()->m_x, position.m_z + m_transform->GetPosition()->m_z), 1.0f);
	//	Matrix4::Multiply(*m_worldMatrix, normal, &normal);
	//	return Vector3::ProjectCubeToSphere(m_radius, position) / m_radius;
		return Vector3::ZERO;
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		Vector3 polar = Vector3::CartesianToPolar(position);
		float value = (polar.m_x - m_radius + AMPLITUDE) / (AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - HALF_SPREAD) * (1.0f / SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / PART));
		float blend = (value - (firstBiome * PART)) / PART;
		Colour colour = Colour();
		Colour::Interpolate(BIOME_COLOURS.at(firstBiome), BIOME_COLOURS.at(firstBiome + 1), blend, &colour);
		return colour;
	}
}
