#include "MeshPattern.hpp"

namespace acid {
MeshPattern::MeshPattern(float sideLength, float squareSize, uint32_t vertexCount, float uvScale) :
	m_sideLength(sideLength),
	m_squareSize(squareSize),
	m_vertexCount(vertexCount),
	m_uvScale(uvScale) {
}

Vertex3 MeshPattern::GetVertex(uint32_t col, uint32_t row) {
	auto x = ((row * m_squareSize) - m_sideLength) / 2.0f;
	auto z = ((col * m_squareSize) - m_sideLength) / 2.0f;

	Vector3f position(x, 0.0f, z);
	Vector2f uv(static_cast<float>(col) * m_uvScale / static_cast<float>(m_vertexCount), static_cast<float>(row) * m_uvScale / static_cast<float>(m_vertexCount));
	Vector3f normal(Vector3f::Up);
	return {position, uv, normal};
}

void MeshPattern::GenerateMesh() {
	std::vector<Vertex3> vertices;
	vertices.reserve(m_vertexCount * m_vertexCount);
	std::vector<uint32_t> indices;
	indices.reserve(6 * (m_vertexCount - 1) * (m_vertexCount - 1));

	// Creates and stores vertices.
	for (uint32_t col = 0; col < m_vertexCount; col++) {
		for (uint32_t row = 0; row < m_vertexCount; row++) {
			vertices.emplace_back(GetVertex(col, row));
		}
	}

	// Creates and stores indices.
	for (uint32_t col = 0; col < m_vertexCount - 1; col++) {
		for (uint32_t row = 0; row < m_vertexCount - 1; row++) {
			auto topLeft = (row * m_vertexCount) + col;
			auto topRight = topLeft + 1;
			auto bottomLeft = ((row + 1) * m_vertexCount) + col;
			auto bottomRight = bottomLeft + 1;
			auto mixed = col % 2 == 0;

			if (row % 2 == 0) {

				indices.emplace_back(mixed ? bottomLeft : topLeft);
				indices.emplace_back(topRight);
				indices.emplace_back(bottomRight);
				indices.emplace_back(mixed ? topRight : bottomRight);
				indices.emplace_back(bottomLeft);
				indices.emplace_back(topLeft);
			} else {
				indices.emplace_back(mixed ? topLeft : topRight);
				indices.emplace_back(bottomRight);
				indices.emplace_back(bottomLeft);
				indices.emplace_back(mixed ? bottomRight : bottomLeft);
				indices.emplace_back(topLeft);
				indices.emplace_back(topRight);
			}
		}
	}

	Initialize(vertices, indices);
}
}
