#include "MeshTerrain.hpp"

#include "../Maths/Maths.hpp"
#include "../Maths/Colour.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	const std::array<Colour, 4> BIOME_COLOURS = {
		Colour("#ffcc00"), Colour("#E17B35"), Colour("#C35D35"), Colour("#683130")
	};
	const float SPREAD = 0.76f;
	const float HALF_SPREAD = SPREAD / 2.0f;
	const float AMPLITUDE = 20.0f;
	const float PART = 1.0f / (BIOME_COLOURS.size() - 1);

	MeshTerrain::MeshTerrain(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, Transform *transform) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_transform(transform),
		m_worldMatrix(new Matrix4())
	{
		m_transform->GetWorldMatrix(m_worldMatrix);
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
		delete m_worldMatrix;
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		Vector4 position = Vector4(x, Terrains::Get()->GetHeight(x + m_transform->GetPosition()->m_x, z + m_transform->GetPosition()->m_z), z, 1.0f);
		Matrix4::Multiply(*m_worldMatrix, position, &position);
		return GetSphereCoords(position);
	}

	Vector3 MeshTerrain::GetNormal(const Vector3 &position)
	{
		Vector4 normal = Vector4(Terrains::Get()->GetNormal(position.m_x + m_transform->GetPosition()->m_x, position.m_z + m_transform->GetPosition()->m_z), 1.0f);
		Matrix4::Multiply(*m_worldMatrix, normal, &normal);
		return normal;
	}

	Vector3 MeshTerrain::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		float value = (position.m_y + AMPLITUDE) / (AMPLITUDE * 2.0f);
		value = Maths::Clamp((value - HALF_SPREAD) * (1.0f / SPREAD), 0.0f, 0.9999f);
		int firstBiome = static_cast<int>(std::floor(value / PART));
		float blend = (value - (firstBiome * PART)) / PART;
		Colour colour = Colour();
		Colour::Interpolate(BIOME_COLOURS.at(firstBiome), BIOME_COLOURS.at(firstBiome + 1), blend, &colour);
		return colour;
	}

	Vector3 MeshTerrain::GetSphereCoords(const Vector3 &position)
	{
		float cs = (3.0f * TerrainRender::SIDE_LENGTH) / 2.0f;
		Vector3 cube = position / cs;
		float sx = cube.m_x * std::sqrt(1.0f - (cube.m_y * cube.m_y / 2.0f) - (cube.m_z * cube.m_z / 2.0f) + (cube.m_y * cube.m_y * cube.m_z * cube.m_z / 3.0f));
		float sy = cube.m_y * std::sqrt(1.0f - (cube.m_z * cube.m_z / 2.0f) - (cube.m_x * cube.m_x / 2.0f) + (cube.m_z * cube.m_z * cube.m_x * cube.m_x / 3.0f));
		float sz = cube.m_z * std::sqrt(1.0f - (cube.m_x * cube.m_x / 2.0f) - (cube.m_y * cube.m_y / 2.0f) + (cube.m_x * cube.m_x * cube.m_y * cube.m_y / 3.0f));
		return Vector3(sx * cs, sy * cs, sz * cs);
	}
}
