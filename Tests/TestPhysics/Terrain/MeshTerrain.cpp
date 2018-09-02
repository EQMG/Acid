#include "MeshTerrain.hpp"

namespace test
{
	MeshTerrain::MeshTerrain(const std::vector<float> &heightmap, const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale) :
		MeshSimple(sideLength, squareSize, vertexCount, textureScale),
		m_heightmap(heightmap)
	{
		fprintf(stdout, "%i\n", (int)m_heightmap.size());
		MeshSimple::GenerateMesh();
	}

	MeshTerrain::~MeshTerrain()
	{
	}

	VertexModel *MeshTerrain::GetVertex(const uint32_t &col, const uint32_t &row)
	{
		float x = ((row * m_squareSize) - m_sideLength) / 2.0f;
		float z = ((col * m_squareSize) - m_sideLength) / 2.0f;

		Vector3 position = GetPosition(x, z);
		Vector2 uv = Vector2(
			static_cast<float>(col) * m_textureScale / static_cast<float>(m_vertexCount),
			static_cast<float>(row) * m_textureScale / static_cast<float>(m_vertexCount)
		);
		Vector3 normal = GetNormal(x, z);
		Colour colour = GetColour(normal);
		return new VertexModel(position, uv, normal, colour);
	}

	Vector3 MeshTerrain::GetPosition(const float &x, const float &z)
	{
		int32_t row = static_cast<int32_t>(((x * 2.0f) + m_sideLength) / m_squareSize);
		int32_t col = static_cast<int32_t>(((z * 2.0f) + m_sideLength) / m_squareSize);

		if (row < 0 || row >= m_vertexCount || col < 0 || col >= m_vertexCount)
		{
			return Vector3(x, 0.0f, z);
		}

		return Vector3(x, m_heightmap[row * m_vertexCount + col], z);
	}

	Vector3 MeshTerrain::GetNormal(const float &x, const float &z)
	{
		Vector3 positionL = GetPosition(x - 1.0f, z);
		Vector3 positionR = GetPosition(x + 1.0f, z);
		Vector3 positionD = GetPosition(x, z - 1.0f);
	//	Vector3 positionU = GetPosition(x, z + 1.0f);

		Vector3 normal = (positionL - positionR).Cross(positionR - positionD);
		return normal.Normalize();
	}

	Colour MeshTerrain::GetColour(const Vector3 &normal)
	{
		return Colour(1.0f, 0.0f, 0.0f, 0.0f);
	}
}
