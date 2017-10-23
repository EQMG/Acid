#include "Terrain.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "../worlds/Worlds.hpp"
#include "Terrains.hpp"
#include "UbosTerrains.hpp"

namespace Flounder
{
	const float Terrain::SIDE_LENGTH = 100.0f;
	const std::vector<float> Terrain::SQUARE_SIZES = { 1.0f, 2.0f, 5.0f, 10.0f, 20.0f };
	const float Terrain::TEXTURE_SCALE = 4.0f;

	Terrain::Terrain(const Vector3 &position, const Vector3 &rotation) :
		m_uniformObject(new UniformBuffer(sizeof(UbosTerrains::UboObject))),
		m_modelLods(std::vector<Model*>()),
		m_textureGrass(new Texture("res/terrains/grass.png")),
		m_textureSnow(new Texture("res/terrains/snow.png")),
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

		// The first LOD is created now for future speed.
		// m_modelLods[0] = GenerateMesh(0);
	}

	Terrain::~Terrain()
	{
		for (auto model : m_modelLods)
		{
			delete model;
		}

		delete m_textureGrass;
		delete m_textureSnow;
		delete m_samplerSand;
		delete m_samplerRock;

		delete m_position;
		delete m_rotation;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void Terrain::Update()
	{
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

		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosTerrains::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.shineDamper = 1.0f;
		uboObject.reflectivity = 0.0f;
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_textureGrass->GetWriteDescriptor(2, descriptorSet), m_textureSnow->GetWriteDescriptor(3, descriptorSet), m_samplerSand->GetWriteDescriptor(4, descriptorSet), m_samplerRock->GetWriteDescriptor(5, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		Vector3 chunkPosition = Vector3(m_aabb->GetCentreX(), m_aabb->GetCentreY(), m_aabb->GetCentreZ());
		chunkPosition.m_y = 0.0f;
		Vector3 cameraPosition = Vector3(*Camera::Get()->GetCamera()->GetPosition());
		cameraPosition.m_y = 0.0f;
		const float distance = Vector3::GetDistance(chunkPosition, cameraPosition);

		// lnreg{ (90.5, 0), (181, 1), (362, 2) } = int(-6.500 + 1.443 * log(x) / log(2.718)) + 1
		//float lodf = floor(-6.5f + 1.443f * log(distance) / log(2.718f)) + 1.0f;

		float lodf = floor(0.0090595f * distance - 1.22865f) + 1.0f;
		lodf = Maths::Clamp(lodf, 0.0f, static_cast<float>(SQUARE_SIZES.size() - 1));
		int lodi = static_cast<int>(lodf);

		if (m_modelLods[lodi] == nullptr)
		{
			m_modelLods[lodi] = GenerateMesh(lodi);

			/*for (int i = 0; i < m_modelLods.size(); i++)
			{
				if (m_modelLods[i] != nullptr && abs(i - lodi) > SQUARE_SIZES.size() / 2)
				{
					delete m_modelLods[i];
				}
			}*/
		}

		m_modelLods[lodi]->CmdRender(commandBuffer);
	}

	int Terrain::CalculateVertexCount(const float &terrainLength, const float &squareSize)
	{
		return static_cast<int>((2.0 * terrainLength) / squareSize) + 2;
	}

	Model *Terrain::GenerateMesh(const int &lod)
	{
#if FLOUNDER_VERBOSE
		auto debugStart = Engine::Get()->GetTimeMs();
#endif
		const float squareSize = SQUARE_SIZES[lod];
		const int vertexCount = CalculateVertexCount(SIDE_LENGTH, squareSize);

		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (int col = 0; col < vertexCount; col++)
		{
			for (int row = 0; row < vertexCount; row++)
			{
				// Creates and stores verticies.
				Vector3 position = Vector3((row * squareSize) - (SIDE_LENGTH / 2.0f), 0.0f, (col * squareSize) - (SIDE_LENGTH / 2.0f));
				position.m_y = Terrains::Get()->GetHeight(position.m_x + m_position->m_x, position.m_z + m_position->m_z); // TODO: Simplify!
				const Vector2 textures = Vector2(
					(TEXTURE_SCALE) * static_cast<float>(col) / static_cast<float>(vertexCount),
					(TEXTURE_SCALE) * static_cast<float>(row) / static_cast<float>(vertexCount)
				);
				const Vector3 normal = CalculateNormal(position.m_x + m_position->m_x, position.m_z + m_position->m_z, 1.5f); // squareSize = constant to make normals uniform.
				const Vector3 tangent = Vector3();

				vertices.push_back(Vertex(position, textures, normal, tangent));

				// Creates and stores indicies.
				if (col < vertexCount - 1 && row < vertexCount - 1)
				{
					const uint32_t topLeft = (row * vertexCount) + col;
					const uint32_t topRight = topLeft + 1;
					const uint32_t bottomLeft = ((row + 1) * vertexCount) + col;
					const uint32_t bottomRight = bottomLeft + 1;

					indices.push_back(topLeft);
					indices.push_back(bottomLeft);
					indices.push_back(topRight);
					indices.push_back(topRight);
					indices.push_back(bottomLeft);
					indices.push_back(bottomRight);
				}
			}
		}

		Model *result = new Model(vertices, indices);

#if FLOUNDER_VERBOSE
		auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Terrain LOD %i took %fms to build!\n", lod, debugEnd - debugStart);
#endif

		return result;
	}

	Vector3 Terrain::CalculateNormal(const float &x, const float &z, const float &squareSize)
	{
		const float heightL = Terrains::Get()->GetHeight(x - squareSize, z);
		const float heightR = Terrains::Get()->GetHeight(x + squareSize, z);
		const float heightD = Terrains::Get()->GetHeight(x, z - squareSize);
		const float heightU = Terrains::Get()->GetHeight(x, z + squareSize);

		Vector3 normal = Vector3(heightL - heightR, squareSize, heightD - heightU);
		normal.Normalize();
		return normal;
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
