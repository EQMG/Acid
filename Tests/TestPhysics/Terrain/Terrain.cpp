#include "Terrain.hpp"

#include <Meshes/Mesh.hpp>
#include <Noise/Noise.hpp>
#include <Physics/ColliderHeightfield.hpp>

namespace test
{
	Terrain::Terrain(const float &sideLength, const float &squareSize) :
		IComponent(),
		m_noise(Noise(25653345)),
		m_sideLength(sideLength),
		m_squareSize(squareSize),
		m_minHeight(+std::numeric_limits<float>::infinity()),
		m_maxHeight(-std::numeric_limits<float>::infinity())
	{
		m_noise.SetNoiseType(NoiseType::TYPE_PERLINFRACTAL);
		m_noise.SetFrequency(0.01f);
		m_noise.SetInterp(NoiseInterp::INTERP_QUINTIC);
		m_noise.SetFractalType(NoiseFractal::FRACTAL_FBM);
		m_noise.SetFractalOctaves(5);
		m_noise.SetFractalLacunarity(2.0f);
		m_noise.SetFractalGain(0.5f);
	}

	Terrain::~Terrain()
	{
	}

	void Terrain::Start()
	{
		auto mesh = GetGameObject()->GetComponent<Mesh>(true);
		auto colliderHeightfield = GetGameObject()->GetComponent<ColliderHeightfield>(true);

		if (mesh == nullptr)
		{
			fprintf(stderr, "Terrain must be attached to a object with a mesh!");
			return;
		}

		uint32_t vertexCount = CalculateVertexCount(m_sideLength, m_squareSize);
		float textureScale = CalculateTextureScale(m_sideLength);
		std::vector<float> heightmap = GenerateHeightmap(vertexCount);
		mesh->SetModel(std::make_shared<MeshTerrain>(heightmap, m_sideLength, m_squareSize, vertexCount, textureScale));

		/*if (colliderHeightfield == nullptr)
		{
			return;
		}

		colliderHeightfield->Initialize(vertexCount, vertexCount, heightmap.data(), 1.0f, m_minHeight, m_maxHeight, false);*/
	}

	void Terrain::Update()
	{
	}

	void Terrain::Load(LoadedValue *value)
	{
	}

	void Terrain::Write(LoadedValue *destination)
	{
	}

	uint32_t Terrain::CalculateVertexCount(const float &sideLength, const float &squareSize)
	{
		return static_cast<uint32_t>((2.0f * sideLength) / static_cast<float>(squareSize)) + 1;
	}

	float Terrain::CalculateTextureScale(const float &sideLength)
	{
		return 0.04f * sideLength;
	}

	std::vector<float> Terrain::GenerateHeightmap(const uint32_t &vertexCount)
	{
		auto transform = GetGameObject()->GetTransform();
		auto heightmap = std::vector<float>(vertexCount * vertexCount);

		for (uint32_t row = 0; row < vertexCount; row++)
		{
			for (uint32_t col = 0; col < vertexCount; col++)
			{
				float x = ((row * m_squareSize) - m_sideLength) / 2.0f;
				float z = ((col * m_squareSize) - m_sideLength) / 2.0f;
				float height = 30.0f * m_noise.GetValueFractal(transform.GetPosition().m_x + x, transform.GetPosition().m_z + z);
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
