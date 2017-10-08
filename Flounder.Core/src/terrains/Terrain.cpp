#include "Terrain.hpp"

#include "../maths/Colour.hpp"
#include "../worlds/Worlds.hpp"
#include "../devices/Display.hpp"
#include "../skyboxes/ShaderSkyboxes.hpp"
#include "ShaderTerrains.hpp"

namespace Flounder
{
	const float Terrain::SQUARE_SIZE = 2.598f;
	const int Terrain::VERTEX_COUNT = 176;
	const float Terrain::SIDE_LENGTH = 0.5f * SQUARE_SIZE * static_cast<float>(VERTEX_COUNT - 1);

	Terrain::Terrain(const Vector3 &position, const Vector3 &rotation, const int &seed) :
		m_uniformObject(new UniformBuffer(sizeof(ShaderTerrains::UboObject))),
		m_model(nullptr),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_moved(true),
		m_modelMatrix(new Matrix4()),
		m_aabb(new Aabb())
	{
		GenerateMesh();
	}

	Terrain::~Terrain()
	{
		delete m_model;

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

	void Terrain::CmdRender(const VkCommandBuffer & commandBuffer, const Pipeline & pipeline, const UniformBuffer & uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this! , m_texture->GetWriteDescriptor(2, descriptorSet)

		ShaderTerrains::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.shineDamper = 1.0f;
		uboObject.reflectivity = 0.0f;
		m_uniformObject->Update(&uboObject);

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkBuffer vertexBuffers[] = { m_model->GetVertexBuffer()->GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, m_model->GetIndexBuffer()->GetBuffer(), 0, m_model->GetIndexBuffer()->GetIndexType());
		vkCmdDrawIndexed(commandBuffer, m_model->GetIndexBuffer()->GetIndexCount(), 1, 0, 0, 0);
	}

	void Terrain::GenerateMesh()
	{
		int count = VERTEX_COUNT * VERTEX_COUNT;
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint16_t> indices = std::vector<uint16_t>();

		const Colour tint = Colour("#51ad5a");

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				Vertex vertex = {};
				vertex.position = Vector3((row * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f), 0.0f, (col * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f));
				vertex.position.m_y = GetHeight(vertex.position.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), vertex.position.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));
				vertex.textures = Vector2();
				vertex.normal = CalculateNormal(vertex.position.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), vertex.position.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));
				vertex.tangent = Vector3(tint.m_r, tint.m_g, tint.m_b);

				vertices.push_back(vertex);
			}
		}

		for (int col = 0; col < VERTEX_COUNT - 1; col++)
		{
			for (int row = 0; row < VERTEX_COUNT - 1; row++)
			{
				int topLeft = (row * VERTEX_COUNT) + col;
				int topRight = topLeft + 1;
				int bottomLeft = ((row + 1) * VERTEX_COUNT) + col;
				int bottomRight = bottomLeft + 1;
				bool mixed = col % 2 == 0;

				if (row % 2 == 0)
				{
					StoreQuad1(indices, topLeft, topRight, bottomLeft, bottomRight, mixed);
				}
				else
				{
					StoreQuad2(indices, topLeft, topRight, bottomLeft, bottomRight, mixed);
				}
			}
		}

		m_model = new Model(vertices, indices);

		m_aabb->m_maxExtents->m_x = SIDE_LENGTH;
		m_aabb->m_maxExtents->m_z = SIDE_LENGTH;
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);

		//	delete vertices;
		//	delete indices;
	}

	void Terrain::StoreQuad1(std::vector<uint16_t> &indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices.push_back(topLeft);
		indices.push_back(bottomLeft);
		indices.push_back(mixed ? topRight : bottomRight);
		indices.push_back(bottomRight);
		indices.push_back(topRight);
		indices.push_back(mixed ? bottomLeft : topLeft);
	}

	void Terrain::StoreQuad2(std::vector<uint16_t> &indices, const int &topLeft, const int &topRight, const int &bottomLeft, const int &bottomRight, const bool &mixed)
	{
		indices.push_back(topRight);
		indices.push_back(topLeft);
		indices.push_back(mixed ? bottomRight : bottomLeft);
		indices.push_back(bottomLeft);
		indices.push_back(bottomRight);
		indices.push_back(mixed ? topLeft : topRight);
	}

	Vector3 Terrain::CalculateNormal(const float &x, const float &z)
	{
		float heightL = GetHeight(x - SQUARE_SIZE, z);
		float heightR = GetHeight(x + SQUARE_SIZE, z);
		float heightD = GetHeight(x, z - SQUARE_SIZE);
		float heightU = GetHeight(x, z + SQUARE_SIZE);

		Vector3 normal = Vector3(heightL - heightR, 2.0f, heightD - heightU);
		normal.Normalize();
		return normal;
	}

	float Terrain::GetHeight(const float &x, const float &z)
	{
		const float worldNoiseSpread = 1.0f;
		const float worldNoiseFrequency = 40.0f;
		const float worldNoiseHeight = 40.0f;

		float height = Worlds::Get()->GetNoise()->GetNoise(x / worldNoiseSpread, z / worldNoiseSpread, worldNoiseFrequency);
		height *= worldNoiseHeight;
		return height;
	}

	void Terrain::SetPosition(const Vector3 &position)
	{
		m_position->Set(position);
		m_moved = true;
	}

	void Terrain::SetRotation(const Vector3 &rotation)
	{
		m_rotation->Set(rotation);
		m_moved = true;
	}
}
