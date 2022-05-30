#include "Terrain.hpp"

#include <Meshes/Mesh.hpp>
#include <Physics/Colliders/HeightfieldCollider.hpp>
#include <Physics/Rigidbody.hpp>

namespace test {
Terrain::Terrain(float sideLength, float squareSize) :
	noise(25653345),
	sideLength(sideLength),
	squareSize(squareSize),
	minHeight(+std::numeric_limits<float>::infinity()),
	maxHeight(-std::numeric_limits<float>::infinity()) {
	noise.SetFrequency(0.01f);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	noise.SetFractalOctaves(5);
	noise.SetFractalLacunarity(2.0f);
	noise.SetFractalGain(0.5f);
}

void Terrain::Start() {
	auto mesh = GetEntity()->GetComponent<Mesh>(true);

	if (!mesh) {
		Log::Error("Terrain must be attached to a object with a mesh!\n");
		return;
	}

	auto vertexCount = CalculateVertexCount(sideLength, squareSize);
	auto textureScale = CalculateTextureScale(sideLength);
	heightmap = GenerateHeightmap(vertexCount);
	mesh->SetModel(std::make_shared<MeshTerrain>(heightmap, sideLength, squareSize, vertexCount, textureScale));

	auto colliderHeightfield = GetEntity()->GetComponent<HeightfieldCollider>(true);

	if (!colliderHeightfield) {
		Log::Error("Terrain does not contain a heightfield collider!\n");
		return;
	}

	colliderHeightfield->SetHeightfield(vertexCount, vertexCount, heightmap.data(), minHeight, maxHeight, true);
}

void Terrain::Update() {
	if (heightmap.empty()) {
		Start();
	}
}

const Node &operator>>(const Node &node, Terrain &terrain) {
	return node;
}

Node &operator<<(Node &node, const Terrain &terrain) {
	return node;
}

uint32_t Terrain::CalculateVertexCount(float sideLength, float squareSize) {
	return static_cast<uint32_t>((2.0f * sideLength) / static_cast<float>(squareSize)) + 1;
}

float Terrain::CalculateTextureScale(float sideLength) {
	return 0.08f * sideLength;
}

std::vector<float> Terrain::GenerateHeightmap(uint32_t vertexCount) {
	auto transform = GetEntity()->GetComponent<Transform>();

	std::vector<float> heightmap(vertexCount * vertexCount);

	for (uint32_t row = 0; row < vertexCount; row++) {
		for (uint32_t col = 0; col < vertexCount; col++) {
			auto x = ((row * squareSize) - sideLength) / 2.0f;
			auto z = ((col * squareSize) - sideLength) / 2.0f;
			float height;

			if (transform) {
				height = 16.0f * noise.GetNoise(transform->GetPosition().x + x, transform->GetPosition().z + z);
			} else {
				height = 16.0f * noise.GetNoise(x, z);
			}

			heightmap[row * vertexCount + col] = height;

			if (height < minHeight) {
				minHeight = height;
			}

			if (height > maxHeight) {
				maxHeight = height;
			}
		}
	}

	return heightmap;
}
}
