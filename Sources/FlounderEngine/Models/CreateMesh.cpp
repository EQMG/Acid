#include <Maths/Maths.hpp>
#include "CreateMesh.hpp"

namespace Flounder
{
	Model *CreateMesh::Create(const float &sideLength, const float &squareSize, const int &vertexCount, const float &textureScale, const MeshType &meshType, const std::function<Vector3(float, float)> &getPosition, const std::function<Vector3(Vector3)> &getNormal, const std::function<Vector3(Vector3, Vector3)> &getColour)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		// Creates and stores vertices.
		for (int col = 0; col < vertexCount; col++)
		{
			for (int row = 0; row < vertexCount; row++)
			{
				// Creates and stores vertices.
				const Vector3 position = getPosition(
					(row * squareSize) - (sideLength / 2.0f),
					(col * squareSize) - (sideLength / 2.0f)
				);
				const Vector2 uv = Vector2(
					textureScale * static_cast<float>(col) / static_cast<float>(vertexCount),
					textureScale * static_cast<float>(row) / static_cast<float>(vertexCount)
				);
				const Vector3 normal = getNormal(position);
				const Vector3 tangent = getColour(position, normal);
				vertices.push_back(Vertex(position, uv, normal, tangent));
			}
		}

		// Creates and stores indices.
		switch (meshType)
		{
			case MeshSimple:
				GenerateSimple(vertexCount, vertices, indices);
				break;
			case MeshPattern:
				GeneratePattern(vertexCount, vertices, indices);
				break;
		}

		return new Model(vertices, indices);
	}

	void CreateMesh::GenerateSimple(const int &vertexCount, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
	{
		// Indices.
		for (int col = 0; col < vertexCount - 1; col++)
		{
			for (int row = 0; row < vertexCount - 1; row++)
			{
				const uint32_t topLeft = (row * vertexCount) + col;
				const uint32_t topRight = topLeft + 1;
				const uint32_t bottomLeft = ((row + 1) * vertexCount) + col;
				const uint32_t bottomRight = bottomLeft + 1;

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
				indices.push_back(topLeft);
				indices.push_back(bottomRight);
				indices.push_back(topRight);
			}
		}
	}

	void CreateMesh::GeneratePattern(const int &vertexCount, std::vector<Vertex> &vertices, std::vector<uint32_t> &indices)
	{
		// Indices.
		for (int col = 0; col < vertexCount - 1; col++)
		{
			for (int row = 0; row < vertexCount - 1; row++)
			{
				const uint32_t topLeft = (row * vertexCount) + col;
				const uint32_t topRight = topLeft + 1;
				const uint32_t bottomLeft = ((row + 1) * vertexCount) + col;
				const uint32_t bottomRight = bottomLeft + 1;
				const bool mixed = col % 2 == 0;

				if (row % 2 == 0)
				{
					indices.push_back(topLeft);
					indices.push_back(bottomLeft);
					indices.push_back(mixed ? topRight : bottomRight);
					indices.push_back(bottomRight);
					indices.push_back(topRight);
					indices.push_back(mixed ? bottomLeft : topLeft);
				}
				else
				{
					indices.push_back(topRight);
					indices.push_back(topLeft);
					indices.push_back(mixed ? bottomRight : bottomLeft);
					indices.push_back(bottomLeft);
					indices.push_back(bottomRight);
					indices.push_back(mixed ? topLeft : topRight);
				}
			}
		}
	}
}
