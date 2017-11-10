#include "Water.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "../maths/Maths.hpp"
#include "Waters.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	const float Water::SIDE_LENGTH = 4096.0f;
	const float Water::SQUARE_SIZE = 128.0f;
	const int Water::VERTEX_COUNT = static_cast<int>((2.0 * SIDE_LENGTH) / SQUARE_SIZE) + 1;
	const float Water::TEXTURE_SCALE = 10.0f;

	const Colour Water::WATER_COLOUR = Colour("#366996");

	Water::Water(const Vector3 &position, const Vector3 &rotation) :
		m_uniformObject(new UniformBuffer(sizeof(UbosWaters::UboObject))),
		m_model(nullptr),
		m_colour(new Colour(WATER_COLOUR)),
		m_textureWater(new Texture("res/waters/water.png")),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_offset(new Vector3()),
		m_moved(true),
		m_modelMatrix(new Matrix4()),
		m_aabb(new Aabb())
	{
		GenerateMesh();
	}

	Water::~Water()
	{
		delete m_model;

		delete m_colour;

		delete m_position;
		delete m_rotation;
		delete m_offset;

		delete m_modelMatrix;
		delete m_aabb;
	}

	void Water::Update()
	{
		if (m_moved)
		{
			Matrix4::TransformationMatrix(*m_position, *m_rotation, 1.0f, m_modelMatrix);
			m_moved = false;
		}

		m_offset->Set(
			2.0f * SQUARE_SIZE * round(Camera::Get()->GetCamera()->GetPosition()->m_x / (2.0f * SQUARE_SIZE)),
			0.0f,
			2.0f * SQUARE_SIZE * round(Camera::Get()->GetCamera()->GetPosition()->m_z / (2.0f * SQUARE_SIZE))
		);
	}

	void Water::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosWaters::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.diffuseColour = Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b,
			Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f);
		uboObject.ignoreReflections = !Waters::Get()->GetEnableReflections();
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_textureWater->GetWriteDescriptor(2, descriptorSet) }; 
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}

	void Water::GenerateMesh()
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<uint32_t> indices = std::vector<uint32_t>();

		for (int col = 0; col < VERTEX_COUNT; col++)
		{
			for (int row = 0; row < VERTEX_COUNT; row++)
			{
				const Vector3 position = Vector3((row * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f), 0.0f, (col * SQUARE_SIZE) - (SIDE_LENGTH / 2.0f));
				const Vector2 textures = Vector2(
					TEXTURE_SCALE * static_cast<float>(col) / static_cast<float>(VERTEX_COUNT),
					TEXTURE_SCALE * static_cast<float>(row) / static_cast<float>(VERTEX_COUNT)
				);
				const Vector3 normal = Vector3(0.0f, 2.0f, 0.0f);
				const Vector3 tangent = Vector3();

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

	void Water::StoreQuad(std::vector<uint32_t> &indices, const uint32_t &topLeft, const uint32_t &topRight, const uint32_t &bottomLeft, const uint32_t &bottomRight)
	{
		indices.push_back(topLeft);
		indices.push_back(bottomLeft);
		indices.push_back(topRight);
		indices.push_back(topRight);
		indices.push_back(bottomLeft);
		indices.push_back(bottomRight);
	}

	void Water::SetPosition(const Vector3 &position)
	{
		m_position->Set(position);
		m_moved = true;
	}

	void Water::SetRotation(const Vector3 &rotation)
	{
		m_rotation->Set(rotation);
		m_moved = true;
	}
}
