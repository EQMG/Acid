#include "MeshTerrain.hpp"

namespace test {
MeshTerrain::MeshTerrain(const std::vector<float> &heightmap, float sideLength, float squareSize, uint32_t vertexCount, float textureScale) :
	SimpleMesh(sideLength, squareSize, vertexCount, textureScale),
	heightmap(heightmap) {
	GenerateMesh();
}

Vertex3d MeshTerrain::GetVertex(uint32_t col, uint32_t row) {
	auto x = ((row * squareSize) - sideLength) / 2.0f;
	auto z = ((col * squareSize) - sideLength) / 2.0f;

	auto position = GetPosition(x, z);
	Vector2f uv(static_cast<float>(col) * uvScale / static_cast<float>(vertexCount), static_cast<float>(row) * uvScale / static_cast<float>(vertexCount));
	auto normal = GetNormal(x, z);
	//auto colour = GetColour(normal);
	return {position, uv, normal};
}

Vector3f MeshTerrain::GetPosition(float x, float z) {
	auto row = static_cast<int32_t>(((x * 2.0f) + sideLength) / squareSize);
	auto col = static_cast<int32_t>(((z * 2.0f) + sideLength) / squareSize);

	if (row < 0 || row >= static_cast<int32_t>(vertexCount) || col < 0 || col >= static_cast<int32_t>(vertexCount)) {
		return {x, 0.0f, z};
	}

	return {x, heightmap[col * vertexCount + row], z};
}

Vector3f MeshTerrain::GetNormal(float x, float z) {
	auto positionL = GetPosition(x - 1.0f, z);
	auto positionR = GetPosition(x + 1.0f, z);
	auto positionD = GetPosition(x, z - 1.0f);
	//auto positionU = GetPosition(x, z + 1.0f);

	auto normal = (positionL - positionR).Cross(positionR - positionD);
	return normal.Normalize();
}

Colour MeshTerrain::GetColour(const Vector3f &normal) {
	return {1.0f, 0.0f, 0.0f, 0.0f};
}
}
