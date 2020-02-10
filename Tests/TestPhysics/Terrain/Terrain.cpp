#include "Terrain.hpp"

#include <Meshes/Mesh.hpp>
#include <Physics/Colliders/ColliderHeightfield.hpp>
#include <Physics/Rigidbody.hpp>

namespace test {
bool Terrain::registered = Register("terrain");

Terrain::Terrain(float sideLength, float squareSize) :
	m_noise(25653345),
	m_sideLength(sideLength),
	m_squareSize(squareSize),
	m_minHeight(+std::numeric_limits<float>::infinity()),
	m_maxHeight(-std::numeric_limits<float>::infinity()) {
	m_noise.SetFrequency(0.01f);
	m_noise.SetInterp(FastNoise::Quintic);
	m_noise.SetNoiseType(FastNoise::PerlinFractal);
	m_noise.SetFractalType(FastNoise::FBM);
	m_noise.SetFractalOctaves(5);
	m_noise.SetFractalLacunarity(2.0f);
	m_noise.SetFractalGain(0.5f);
}

void Terrain::Start() {
	auto mesh = GetEntity()->GetComponent<Mesh>(true);

	if (!mesh) {
		Log::Error("Terrain must be attached to a object with a mesh!\n");
		return;
	}

	auto vertexCount = CalculateVertexCount(m_sideLength, m_squareSize);
	auto textureScale = CalculateTextureScale(m_sideLength);
	m_heightmap = GenerateHeightmap(vertexCount);
	mesh->SetModel(std::make_shared<MeshTerrain>(m_heightmap, m_sideLength, m_squareSize, vertexCount, textureScale));

	auto colliderHeightfield = GetEntity()->GetComponent<ColliderHeightfield>(true);

	if (!colliderHeightfield) {
		Log::Error("Terrain does not contain a heightfield collider!\n");
		return;
	}

	colliderHeightfield->SetHeightfield(vertexCount, vertexCount, m_heightmap.data(), m_minHeight, m_maxHeight, true);
}

void Terrain::Update() {
	if (m_heightmap.empty()) {
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
			auto x = ((row * m_squareSize) - m_sideLength) / 2.0f;
			auto z = ((col * m_squareSize) - m_sideLength) / 2.0f;
			float height;

			if (transform) {
				height = 16.0f * m_noise.GetValueFractal(transform->GetPosition().m_x + x, transform->GetPosition().m_z + z);
			} else {
				height = 16.0f * m_noise.GetValueFractal(x, z);
			}

			heightmap[row * vertexCount + col] = height;

			if (height < m_minHeight) {
				m_minHeight = height;
			}

			if (height > m_maxHeight) {
				m_maxHeight = height;
			}
		}
	}

	return heightmap;
}
}
