#include "MeshPattern.hpp"

namespace acid
{
MeshPattern::MeshPattern(const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale) :
	m_sideLength(sideLength),
	m_squareSize(squareSize),
	m_vertexCount(vertexCount),
	m_textureScale(textureScale)
{
}

void MeshPattern::GenerateMesh()
{
	std::vector<VertexModel> vertices;
	std::vector<uint32_t> indices;
	// TODO: Reserve.

	// Creates and stores vertices.
	for (uint32_t col = 0; col < m_vertexCount; col++)
	{
		for (uint32_t row = 0; row < m_vertexCount; row++)
		{
			vertices.emplace_back(GetVertex(col, row));
		}
	}

	// Creates and stores indices.
	for (uint32_t col = 0; col < m_vertexCount - 1; col++)
	{
		for (uint32_t row = 0; row < m_vertexCount - 1; row++)
		{
			auto topLeft = (row * m_vertexCount) + col;
			auto topRight = topLeft + 1;
			auto bottomLeft = ((row + 1) * m_vertexCount) + col;
			auto bottomRight = bottomLeft + 1;
			bool mixed = col % 2 == 0;

			if (row % 2 == 0) // TODO: Reverse order.
			{
				indices.emplace_back(topLeft);
				indices.emplace_back(bottomLeft);
				indices.emplace_back(mixed ? topRight : bottomRight);
				indices.emplace_back(bottomRight);
				indices.emplace_back(topRight);
				indices.emplace_back(mixed ? bottomLeft : topLeft);
			}
			else
			{
				indices.emplace_back(topRight);
				indices.emplace_back(topLeft);
				indices.emplace_back(mixed ? bottomRight : bottomLeft);
				indices.emplace_back(bottomLeft);
				indices.emplace_back(bottomRight);
				indices.emplace_back(mixed ? topLeft : topRight);
			}
		}
	}

	Initialize(vertices, indices);
}

VertexModel MeshPattern::GetVertex(const uint32_t &col, const uint32_t &row)
{
	auto x = ((row * m_squareSize) - m_sideLength) / 2.0f;
	auto z = ((col * m_squareSize) - m_sideLength) / 2.0f;

	auto position = Vector3(x, 0.0f, z);
	auto uv = Vector2(static_cast<float>(col) * m_textureScale / static_cast<float>(m_vertexCount), static_cast<float>(row) * m_textureScale / static_cast<float>(m_vertexCount));
	auto normal = Vector3::Up;
	//auto colour = Colour::White;
	return VertexModel(position, uv, normal); // , colour
}
}
