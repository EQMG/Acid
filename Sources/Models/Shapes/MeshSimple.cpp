#include "MeshSimple.hpp"

namespace acid
{
	MeshSimple::MeshSimple(const float &sideLength, const float &squareSize, const uint32_t &vertexCount, const float &textureScale) :
		Model(),
		m_sideLength(sideLength),
		m_squareSize(squareSize),
		m_vertexCount(vertexCount),
		m_textureScale(textureScale)
	{
	}

	void MeshSimple::GenerateMesh()
	{
		std::vector<VertexModel> vertices = {};
		std::vector<uint32_t> indices = {};

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
				uint32_t topLeft = (row * m_vertexCount) + col;
				uint32_t topRight = topLeft + 1;
				uint32_t bottomLeft = ((row + 1) * m_vertexCount) + col;
				uint32_t bottomRight = bottomLeft + 1;

				indices.emplace_back(topLeft);
				indices.emplace_back(bottomLeft);
				indices.emplace_back(topRight);
				indices.emplace_back(topRight);
				indices.emplace_back(bottomLeft);
				indices.emplace_back(bottomRight);
			}
		}

		Model::Initialize(vertices, indices);
	}

	VertexModel MeshSimple::GetVertex(const uint32_t &col, const uint32_t &row)
	{
		float x = ((row * m_squareSize) - m_sideLength) / 2.0f;
		float z = ((col * m_squareSize) - m_sideLength) / 2.0f;

		Vector3 position = Vector3(x, 0.0f, z);
		Vector2 uv = Vector2(
			static_cast<float>(col) * m_textureScale / static_cast<float>(m_vertexCount),
			static_cast<float>(row) * m_textureScale / static_cast<float>(m_vertexCount)
		);
		Vector3 normal = Vector3::UP;
		Colour colour = Colour::WHITE;
		return VertexModel(position, uv, normal, colour); 
	}
}
