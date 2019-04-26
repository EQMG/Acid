#include "Terrain.hpp"

#include <Meshes/Mesh.hpp>
#include <Physics/Colliders/ColliderHeightfield.hpp>
#include <Physics/Rigidbody.hpp>

namespace test
{
Terrain::Terrain(const float &sideLength, const float &squareSize) :
	m_noise(25653345, 0.01f, Noise::Interp::Quintic, Noise::Type::PerlinFractal, 5, 2.0f, 0.5f, Noise::Fractal::FBM),
	m_sideLength(sideLength),
	m_squareSize(squareSize),
	m_minHeight(+std::numeric_limits<float>::infinity()),
	m_maxHeight(-std::numeric_limits<float>::infinity())
{
}

void Terrain::Start()
{
	auto mesh = GetParent()->GetComponent<Mesh>(true);

	if (mesh == nullptr)
	{
		Log::Error("Terrain must be attached to a object with a mesh!\n");
		return;
	}

	uint32_t vertexCount = CalculateVertexCount(m_sideLength, m_squareSize);
	float textureScale = CalculateTextureScale(m_sideLength);
	m_heightmap = GenerateHeightmap(vertexCount);
	mesh->SetModel(std::make_shared<MeshTerrain>(m_heightmap, m_sideLength, m_squareSize, vertexCount, textureScale));

	auto colliderHeightfield = GetParent()->GetComponent<ColliderHeightfield>(true);

	if (colliderHeightfield == nullptr)
	{
		Log::Error("Terrain does not contail a heightfield collider!\n");
		return;
	}

	colliderHeightfield->Initialize(vertexCount, vertexCount, m_heightmap.data(), m_minHeight, m_maxHeight, true);
}

void Terrain::Update()
{
	if (m_heightmap.empty())
	{
		Start();
	}
}

const Metadata& operator>>(const Metadata& metadata, Terrain& terrain)
{
	return metadata;
}

Metadata& operator<<(Metadata& metadata, const Terrain& terrain)
{
	return metadata;
}

uint32_t Terrain::CalculateVertexCount(const float &sideLength, const float &squareSize)
{
	return static_cast<uint32_t>((2.0f * sideLength) / static_cast<float>(squareSize)) + 1;
}

float Terrain::CalculateTextureScale(const float &sideLength)
{
	return 0.08f * sideLength;
}

std::vector<float> Terrain::GenerateHeightmap(const uint32_t &vertexCount)
{
	auto &transform = GetParent()->GetLocalTransform();
	auto heightmap = std::vector<float>(vertexCount * vertexCount);

	for (uint32_t row = 0; row < vertexCount; row++)
	{
		for (uint32_t col = 0; col < vertexCount; col++)
		{
			float x = ((row * m_squareSize) - m_sideLength) / 2.0f;
			float z = ((col * m_squareSize) - m_sideLength) / 2.0f;
			float height = 16.0f * m_noise.GetValueFractal(transform.GetPosition().m_x + x, transform.GetPosition().m_z + z);
			heightmap[row * vertexCount + col] = height;

			if (height < m_minHeight)
			{
				m_minHeight = height;
			}

			if (height > m_maxHeight)
			{
				m_maxHeight = height;
			}
		}
	}

	return heightmap;
}
}
