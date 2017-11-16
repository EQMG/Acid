#include "Water.hpp"

#include "../camera/Camera.hpp"
#include "../devices/Display.hpp"
#include "../maths/Maths.hpp"
#include "../models/MeshGenerator.hpp"
#include "../terrains/Terrains.hpp"
#include "../renderer/Renderer.hpp"
#include "Waters.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	const float Water::SIDE_LENGTH = 2048.0f;
	const float Water::SQUARE_SIZE = 100.0f;
	const int Water::VERTEX_COUNT = static_cast<int>((2.0 * SIDE_LENGTH) / SQUARE_SIZE) + 1;
	const float Water::TEXTURE_SCALE = 1.0f;

	const Colour Water::WATER_COLOUR = Colour("#366996");
	const float Water::WAVE_SPEED = 0.0003f;

	Water::Water(const Vector3 &position, const Vector3 &rotation) :
		m_uniformObject(new UniformBuffer(sizeof(UbosWaters::UboObject))),
		m_model(nullptr),
		m_colour(new Colour(WATER_COLOUR)),
		m_textureDudv(new Texture("res/waters/dudv.png")),
		m_position(new Vector3(position)),
		m_rotation(new Vector3(rotation)),
		m_moveFactor(0.0f),
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

		m_moveFactor += WAVE_SPEED * Engine::Get()->GetDelta();
		m_moveFactor = fmod(m_moveFactor, 1.0f);
	}

	void Water::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		UbosWaters::UboObject uboObject = {};
		uboObject.transform = Matrix4(*m_modelMatrix);
		uboObject.diffuseColour = Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b,
			Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f);
		uboObject.cameraPosition = Vector3(*Camera::Get()->GetCamera()->GetPosition());
		uboObject.moveFactor = m_moveFactor;
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet), m_textureDudv->GetWriteDescriptor(2, descriptorSet), Renderer::Get()->GetSwapchain()->GetShadowImage()->GetWriteDescriptor(3, descriptorSet) };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = { pipeline.GetDescriptorSet() };
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}

	void Water::GenerateMesh()
	{
		m_model = MeshGenerator::GenerateMesh(SIDE_LENGTH, SQUARE_SIZE, VERTEX_COUNT, TEXTURE_SCALE, MeshType::MeshPattern, [&](float x, float z)
		{
			return 0.0f;
		});

		m_aabb->m_maxExtents->m_x = SIDE_LENGTH;
		m_aabb->m_maxExtents->m_z = SIDE_LENGTH;
		m_position->m_x -= m_aabb->m_maxExtents->m_x / 2.0f;
		m_position->m_z -= m_aabb->m_maxExtents->m_z / 2.0f;
		m_aabb->Update(*m_position, *m_rotation, 1.0f, m_aabb);
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
