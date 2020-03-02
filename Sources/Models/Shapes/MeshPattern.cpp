#include "MeshPattern.hpp"

namespace acid {
MeshPattern::MeshPattern(float sideLength, float squareSize, uint32_t vertexCount, float uvScale) :
	sideLength(sideLength),
	squareSize(squareSize),
	vertexCount(vertexCount),
	uvScale(uvScale) {
}

Vertex3d MeshPattern::GetVertex(uint32_t col, uint32_t row) {
	auto x = ((row * squareSize) - sideLength) / 2.0f;
	auto z = ((col * squareSize) - sideLength) / 2.0f;

	Vector3f position(x, 0.0f, z);
	Vector2f uv(static_cast<float>(col) * uvScale / static_cast<float>(vertexCount), static_cast<float>(row) * uvScale / static_cast<float>(vertexCount));
	Vector3f normal(Vector3f::Up);
	return {position, uv, normal};
}

void MeshPattern::GenerateMesh() {
	std::vector<Vertex3d> vertices;
	vertices.reserve(vertexCount * vertexCount);
	std::vector<uint32_t> indices;
	indices.reserve(6 * (vertexCount - 1) * (vertexCount - 1));

	// Creates and stores vertices.
	for (uint32_t col = 0; col < vertexCount; col++) {
		for (uint32_t row = 0; row < vertexCount; row++) {
			vertices.emplace_back(GetVertex(col, row));
		}
	}

	// Creates and stores indices.
	for (uint32_t col = 0; col < vertexCount - 1; col++) {
		for (uint32_t row = 0; row < vertexCount - 1; row++) {
			auto topLeft = (row * vertexCount) + col;
			auto topRight = topLeft + 1;
			auto bottomLeft = ((row + 1) * vertexCount) + col;
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
