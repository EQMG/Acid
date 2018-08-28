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
		m_squareSize(squareSize)
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
	//	auto colliderHeightfield = GetGameObject()->GetComponent<ColliderHeightfield>(true);

		if (mesh == nullptr)
		{
			fprintf(stderr, "Terrain must be attached to a object with a mesh!");
			return;
		}

		uint32_t vertexCount = CalculateVertexCount(m_sideLength, m_squareSize);
		float textureScale = CalculateTextureScale(m_sideLength);
		auto transform = GetGameObject()->GetTransform();
		mesh->SetModel(std::make_shared<MeshTerrain>(&m_noise, m_sideLength, m_squareSize, vertexCount, textureScale, transform));

		/*if (colliderHeightfield == nullptr)
		{
			return;
		}

		void *data = malloc(sizeof(float) * m_sideLength * m_sideLength);
		float heightScale = 1.0f;
		float minHeight = 0.0f;
		float maxHeight = 0.0f;
		colliderHeightfield->Initialize(m_sideLength, m_sideLength, data, heightScale, minHeight, maxHeight, false);*/
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
		return 0.1f * sideLength;
	}
}
