#include "MeshSimple.hpp"

#include "Models/VertexModel.hpp"

namespace acid
{
	MeshSimple::MeshSimple(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale) :
		Model(),
		m_sideLength(sideLength),
		m_squareSize(squareSize),
		m_vertexCount(vertexCount),
		m_textureScale(textureScale)
	{
	}

	void MeshSimple::GenerateMesh()
	{
		auto vertices = std::vector<IVertex *>();
		auto indices = std::vector<uint32_t>();

		// Creates and stores vertices.
		for (int col = 0; col < m_vertexCount; col++)
		{
			for (int row = 0; row < m_vertexCount; row++)
			{
				float x = ((row * m_squareSize) - m_sideLength) / 2.0f;
				float z = ((col * m_squareSize) - m_sideLength) / 2.0f;

				Vector3 position = GetPosition(x, z);
				Vector2 uv = Vector2(
					static_cast<float>(col) * m_textureScale / static_cast<float>(m_vertexCount),
					static_cast<float>(row) * m_textureScale / static_cast<float>(m_vertexCount)
				);
				Vector3 normal = GetNormal(x, z, position);
				Vector3 colour = GetColour(position, normal);
				vertices.emplace_back(new VertexModel(position, uv, normal, colour));
			}
		}

		// Creates and stores indices.
		for (int col = 0; col < m_vertexCount - 1; col++)
		{
			for (int row = 0; row < m_vertexCount - 1; row++)
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

		Model::Set(vertices, indices);
	}

	Vector3 MeshSimple::GetPosition(const float &x, const float &z)
	{
		return Vector3(x, 0.0f, z);
	}

	Vector3 MeshSimple::GetNormal(const float &x, const float &z, const Vector3 &position)
	{
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	Vector3 MeshSimple::GetColour(const Vector3 &position, const Vector3 &normal)
	{
		return Vector3(1.0f, 1.0f, 1.0f);
	}
}
