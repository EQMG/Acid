#include "Terrain.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "../worlds/Worlds.hpp"
#include "Terrains.hpp"
#include "UbosTerrains.hpp"
#include "../models/MeshGenerator.hpp"

namespace Flounder
{
	const float Terrain::SIDE_LENGTH = 100.0f;
	const std::vector<float> Terrain::SQUARE_SIZES = { 2.0f, 5.0f, 10.0f, 20.0f }; // Models: (1.0 LOD, 2/5 LOD, 1/10 LOD, none)
	const std::vector<float> Terrain::TEXTURE_SCALES = { 10.0f, 5.0f, 2.0f, 1.0f };

	Terrain::Terrain(const Vector3 &position, const Vector3 &rotation) :
		m_uniformObject(new UniformBuffer(sizeof(UbosTerrains::UboObject))),
		m_modelLods(std::vector<Model*>()),
		m_currentLod(0),
		m_textureGrass(new Texture("res/terrains/grass.png")),
		m_samplerSand(new Texture("res/terrains/sand.png")),
		m_samplerRock(new Texture("res/terrains/rock.png")),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_moved(true),
		m_modelMatrix(new Matrix4()),
		m_aabb(new Aabb())
	{
		m_aabb->m_maxExtents->m_x = SIDE_LENGTH;
		m_aabb->m_maxExtents->m_z = SIDE_LENGTH;
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);

		for (int i = 0; i < SQUARE_SIZES.size(); i++)
		{
			m_modelLods.push_back(nullptr);
		}
	}

	Terrain::~Terrain()
	{
		for (auto model : m_modelLods)
		{
			delete model;
		}

		delete m_textureGrass;
		delete m_samplerSand;
		delete m_samplerRock;

		delete m_position;
		delete m_rotation;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void Terrain::Update()
	{
		Vector3 cameraPosition = Vector3(*Camera::Get()->GetCamera()->GetPosition());
	//	cameraPosition.m_y = 0.0f;
		Vector3 chunkPosition = Vector3(m_aabb->GetCentreX(), m_aabb->GetCentreY(), m_aabb->GetCentreZ());
		chunkPosition.m_y = Terrains::Get()->GetHeight(cameraPosition.m_x, cameraPosition.m_z);
		const float distance = Vector3::GetDistance(chunkPosition, cameraPosition);

		// lnreg{ (90.5, 0), (181, 1), (362, 2) } = int(-6.500 + 1.443 * log(x) / log(2.718)) + 1
		// float lodf = floor(-6.5f + 1.443f * log(distance) / log(2.718f)) + 1.0f;
		float lod = floor(0.0090595f * distance - 1.22865f) + 1.0f;
		lod = Maths::Clamp(lod, 0.0f, static_cast<float>(SQUARE_SIZES.size() - 1));
		m_currentLod = static_cast<int>(lod);

		if (m_modelLods[m_currentLod] == nullptr)
		{
			/*for (int i = 0; i < m_modelLods.size(); i++)
			{
			if (m_modelLods[i] != nullptr && abs(i - lodi) > SQUARE_SIZES.size() / 2)
			{
			delete m_modelLods[i];
			}
			}*/

			CreateLod(m_currentLod);
		}

		if (m_moved)
		{
			Matrix4::TransformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}
	}

	void Terrain::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		//if (!m_aabb->InFrustum(*Camera::Get()->GetCamera()->GetViewFrustum()))
		//{
		//	return;
		//}

		if (m_modelLods[m_currentLod] == nullptr)
		{
			return;
		}

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosTerrains::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_textureGrass->GetWriteDescriptor(2, descriptorSet), m_samplerSand->GetWriteDescriptor(3, descriptorSet), m_samplerRock->GetWriteDescriptor(4, descriptorSet) };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_modelLods[m_currentLod]->CmdRender(commandBuffer);
	}

	int Terrain::CalculateVertexCount(const float &terrainLength, const float &squareSize)
	{
		return static_cast<int>((2.0 * terrainLength) / squareSize) + 1;
	}

	void Terrain::CreateLod(const int &lod)
	{
		if (m_modelLods[lod] != nullptr)
		{
			return;
		}

#if FLOUNDER_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif
		const float squareSize = SQUARE_SIZES[lod];
		const float textureScale = TEXTURE_SCALES[lod];
		const int vertexCount = CalculateVertexCount(SIDE_LENGTH, squareSize);

		m_modelLods[lod] = MeshGenerator::GenerateMesh(SIDE_LENGTH, squareSize, vertexCount, textureScale, MeshType::MeshPattern, [&](float x, float z)
		{
			return Terrains::Get()->GetHeight(x + m_position->m_x, z + m_position->m_z);
		});

#if FLOUNDER_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Terrain LOD %i took %fms to build!\n", lod, debugEnd - debugStart);
#endif
	}

	void Terrain::SetPosition(const Vector3 &position)
	{
		m_position->Set(position);
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_moved = true;
	}

	void Terrain::SetRotation(const Vector3 &rotation)
	{
		m_rotation->Set(rotation);
		m_moved = true;
	}
}
