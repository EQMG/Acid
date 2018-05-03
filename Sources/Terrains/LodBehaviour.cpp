#include "LodBehaviour.hpp"

#include "Scenes/Scenes.hpp"
#include "Maths/Maths.hpp"
#include "Meshes/Mesh.hpp"
#include "MeshTerrain.hpp"

namespace fl
{
	LodBehaviour::LodBehaviour(const float &radius, const Transform &transform) :
		Behaviour(),
		m_radius(radius),
		m_transform(new Transform(transform)),
		m_modelLods(std::vector<Model *>()),
		m_currentLod(5)
	{
		for (int i = 0; i < static_cast<int>(MeshTerrain::SQUARE_SIZES.size()); i++)
		{
			m_modelLods.push_back(nullptr);
		}
	}

	LodBehaviour::~LodBehaviour()
	{
		delete m_transform;

		for (auto model : m_modelLods)
		{
			delete model;
		}
	}

	void LodBehaviour::Update()
	{
		Vector3 cameraPosition = Vector3(*Scenes::Get()->GetCamera()->GetPosition());
		Vector3 chunkPosition = m_transform->GetPosition()->ProjectCubeToSphere(m_radius) + *GetGameObject()->GetTransform()->GetPosition();
		float distance = std::fabs(chunkPosition.Distance(cameraPosition));

		// lnreg{ (90.5, 0), (181, 1), (362, 2) } = int(-6.500 + 1.443 * log(x) / log(2.718)) + 1
		// float lodf = floor(-6.5f + 1.443f * log(distance) / log(2.718f)) + 1.0f;
		float lod = std::floor(0.0090595f * distance - 1.22865f) + 1.0f;
		lod = Maths::Clamp(lod, 0.0f, static_cast<float>(MeshTerrain::SQUARE_SIZES.size() - 1));
		unsigned int lodi = static_cast<unsigned int>(lod);

		if (lodi != m_currentLod)
		{
			if (m_modelLods.at(lodi) == nullptr)
			{
				CreateLod(lodi);
			}

			auto mesh = GetGameObject()->GetComponent<Mesh>();

			if (mesh != nullptr)
			{
				mesh->SetModel(m_modelLods.at(lodi));
			}

			m_currentLod = lodi;
		}
	}

	void LodBehaviour::CreateLod(const unsigned int &lod)
	{
		if (m_modelLods.at(lod) != nullptr)
		{
			return;
		}

#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif
		float squareSize = MeshTerrain::SQUARE_SIZES.at(lod);
		float textureScale = MeshTerrain::TEXTURE_SCALES.at(lod);
		int vertexCount = CalculateVertexCount(MeshTerrain::SIDE_LENGTH, squareSize);
		float lodFixScale = 1.0f; // (lod == 0) ? 1.0f : 1.02f + (0.028f * lod);
		m_modelLods.at(lod) = new MeshTerrain(lodFixScale * static_cast<float>(MeshTerrain::SIDE_LENGTH), lodFixScale * squareSize, vertexCount, textureScale, m_radius, m_transform);
#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();

		if (debugEnd - debugStart > 22.0f)
		{
			printf("Terrain LOD %i built in %fms\n", lod, debugEnd - debugStart);
		}
#endif
	}

	int LodBehaviour::CalculateVertexCount(const int &terrainLength, const float &squareSize)
	{
		return static_cast<int>((2.0 * terrainLength) / static_cast<float>(squareSize)) + 1;
	}
}
