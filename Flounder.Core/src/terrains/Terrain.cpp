#include "Terrain.hpp"

#include "../devices/Display.hpp"
#include "../skyboxes/ShaderSkyboxes.hpp"
#include "../worlds/Worlds.hpp"
#include "ShaderTerrains.hpp"
#include "Terrains.hpp"

namespace Flounder
{
	const float Terrain::SIDE_LENGTH = 512.0f;
	const float Terrain::SQUARE_SIZE = 2.0f;
	const int Terrain::VERTEX_COUNT = static_cast<int>((2.0 * SIDE_LENGTH) / SQUARE_SIZE) + 1;

	Terrain::Terrain(const Vector3 &position, const Vector3 &rotation) :
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

	void Terrain::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		ShaderTerrains::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.shineDamper = 1.0f;
		uboObject.reflectivity = 0.0f;
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet) }; // TODO: Modulaize this!
		VkDescriptorSet descriptors[] = { pipeline.GetDescriptorSet() };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);
		
		m_model->CmdRender(commandBuffer);
	}

	void Terrain::GenerateMesh()
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				Vector3 position = Vector3((row * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f), 0.0f, (col * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f));
				position.m_y = Terrains::Get()->GetHeight(position.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), position.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f)); // TODO: Simplify!
				const Vector2 textures = Vector2();
				const Vector3 normal = CalculateNormal(position.m_x + m_position->m_x - (SIDE_LENGTH / 2.0f), position.m_z + m_position->m_z - (SIDE_LENGTH / 2.0f));
				const Vector3 tangent = Vector3(CalculateColour(position, normal));

				vertices.push_back(Vertex(position, textures, normal, tangent));
			}
		}

		for (int col = 0; col < VERTEX_COUNT - 1; col++)
		{
			for (int row = 0; row < VERTEX_COUNT - 1; row++)
			{
				const uint32_t topLeft = (row * VERTEX_COUNT) + col;
				const uint32_t topRight = topLeft + 1;
				const uint32_t bottomLeft = ((row + 1) * VERTEX_COUNT) + col;
				const uint32_t bottomRight = bottomLeft + 1;

				StoreQuad(indices, topLeft, topRight, bottomLeft, bottomRight);
			}
		}

		m_model = new Model(vertices, indices);

		m_aabb->m_maxExtents->m_x = SIDE_LENGTH;
		m_aabb->m_maxExtents->m_z = SIDE_LENGTH;
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);
	}

	void Terrain::StoreQuad(std::vector<uint32_t> &indices, const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight)
	{
		indices.push_back(topLeft);
		indices.push_back(bottomLeft);
		indices.push_back(topRight);
		indices.push_back(topRight);
		indices.push_back(bottomLeft);
		indices.push_back(bottomRight);
	}

	Vector3 Terrain::CalculateNormal(const float &x, const float &z)
	{
		const float heightL = Terrains::Get()->GetHeight(x - SQUARE_SIZE, z);
		const float heightR = Terrains::Get()->GetHeight(x + SQUARE_SIZE, z);
		const float heightD = Terrains::Get()->GetHeight(x, z - SQUARE_SIZE);
		const float heightU = Terrains::Get()->GetHeight(x, z + SQUARE_SIZE);

		Vector3 normal = Vector3(heightL - heightR, 2.0f, heightD - heightU);
		normal.Normalize();
		return normal;
	}

	Colour Terrain::CalculateColour(const Vector3 &position, const Vector3 &normal)
	{
		const Colour tintGrass = Colour("#51ad5a");
		const Colour tintRock = Colour("#6D695E");
		const Colour tintSand = Colour("#F7D8AC");
		const Colour tintSnow = Colour("#ffffff");
		const float heightFactor = position.m_y;

		Colour tint;

		if (heightFactor <= 0.5f)
		{
			tint = tintSand;
		}
		else if (heightFactor >= 23.0f)
		{
			tint = tintSnow;
		}
		else
		{
			tint = tintGrass;
		}

		Colour::Interpolate(tintRock, tint, Maths::Clamp(fabs(normal.m_y), 0.0f, 1.0f), &tint);
		return tint;
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
