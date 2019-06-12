#include "MeshTerrain.hpp"

namespace test
{
MeshTerrain::MeshTerrain(const std::vector<float> &heightmap, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale) :
	MeshSimple{sideLength, squareSize, vertexCount, textureScale},
	m_heightmap{heightmap}
{
	GenerateMesh();
}

VertexDefault MeshTerrain::GetVertex(const uint32_t &col, const uint32_t &row)
{
	auto x{((row * m_squareSize) - m_sideLength) / 2.0f};
	auto z{((col * m_squareSize) - m_sideLength) / 2.0f};

	auto position{GetPosition(x, z)};
	Vector2f uv{static_cast<float>(col) * m_uvScale / static_cast<float>(m_vertexCount), static_cast<float>(row) * m_uvScale / static_cast<float>(m_vertexCount)};
	auto normal{GetNormal(x, z)};
	//auto colour{GetColour(normal)};
	return {position, uv, normal};
}

Vector3f MeshTerrain::GetPosition(const float &x, const float &z)
{
	auto row{static_cast<int32_t>(((x * 2.0f) + m_sideLength) / m_squareSize)};
	auto col{static_cast<int32_t>(((z * 2.0f) + m_sideLength) / m_squareSize)};

	if (row < 0 || row >= static_cast<int32_t>(m_vertexCount) || col < 0 || col >= static_cast<int32_t>(m_vertexCount))
	{
		return {x, 0.0f, z};
	}

	return {x, m_heightmap[col * m_vertexCount + row], z};
}

Vector3f MeshTerrain::GetNormal(const float &x, const float &z)
{
	auto positionL{GetPosition(x - 1.0f, z)};
	auto positionR{GetPosition(x + 1.0f, z)};
	auto positionD{GetPosition(x, z - 1.0f)};
	//auto positionU{GetPosition(x, z + 1.0f)};

	auto normal{(positionL - positionR).Cross(positionR - positionD)};
	return normal.Normalize();
}

Colour MeshTerrain::GetColour(const Vector3f &normal)
{
	return {1.0f, 0.0f, 0.0f, 0.0f};
}
}
