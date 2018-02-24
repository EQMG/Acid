#include "WaterRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "Waters.hpp"
#include "UbosWaters.hpp"

namespace Flounder
{
	const float WaterRender::SIDE_LENGTH = 3072.0f;
	const float WaterRender::SQUARE_SIZE = 1024.0f;
	const int WaterRender::VERTEX_COUNT = static_cast<int>((2.0f * SIDE_LENGTH) / SQUARE_SIZE) + 1;
	const float WaterRender::TEXTURE_SCALE = 1.0f;

	const Colour WaterRender::WATER_COLOUR = Colour("#0077be");

	WaterRender::WaterRender() :
		m_uniformObject(new UniformBuffer(sizeof(UbosWaters::UboObject))),
		m_colour(new Colour(WATER_COLOUR))
	{
	}

	WaterRender::~WaterRender()
	{
		delete m_colour;
	}

	void WaterRender::Update()
	{
	}

	void WaterRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		// Gets required components.
		auto mesh = GetGameObject()->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Creates a UBO object and write descriptor.
		UbosWaters::UboObject uboObject = {};
		GetGameObject()->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.diffuseColour = Colour(m_colour->m_r, m_colour->m_g, m_colour->m_b, Waters::Get()->GetEnableReflections() ? Waters::Get()->GetColourIntensity() : 1.0f);
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
			uniformScene.GetWriteDescriptor(0, descriptorSet),
			m_uniformObject->GetWriteDescriptor(1, descriptorSet)
		//	Renderer::Get()->GetSwapchain()->GetShadowImage()->GetWriteDescriptor(3, descriptorSet)
		};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		// Draws the object.
		VkDescriptorSet descriptors[] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
