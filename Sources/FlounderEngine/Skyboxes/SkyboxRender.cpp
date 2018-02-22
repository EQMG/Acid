#include <Worlds/Worlds.hpp>
#include "SkyboxRender.hpp"

#include "../Devices/Display.hpp"
#include "../Meshes/Mesh.hpp"
#include "Skyboxes.hpp"
#include "UbosSkyboxes.hpp"

namespace Flounder
{
	SkyboxRender::SkyboxRender(Cubemap *cubemap) :
		Component(),
		m_uniformObject(new UniformBuffer(sizeof(UbosSkyboxes::UboObject))),
		m_cubemap(cubemap),
		m_blend(1.0f)
	{
	}

	SkyboxRender::~SkyboxRender()
	{
		delete m_cubemap;
	}

	void SkyboxRender::Update()
	{
		m_gameObject->GetTransform()->SetRotation(*Worlds::Get()->GetSkyboxRotation());
		m_blend = Worlds::Get()->GetStarIntensity();
	}

	void SkyboxRender::CmdRender(const VkCommandBuffer &commandBuffer, const Pipeline &pipeline, const UniformBuffer &uniformScene)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = pipeline.GetDescriptorSet();

		// Gets required components.
		auto mesh = m_gameObject->GetComponent<Mesh>();

		if (mesh == nullptr || mesh->GetModel() == nullptr)
		{
			return;
		}

		// Creates a UBO object and write descriptor.
		UbosSkyboxes::UboObject uboObject = {};
		m_gameObject->GetTransform()->GetWorldMatrix(&uboObject.transform);
		uboObject.skyColour = Colour(*Skyboxes::Get()->GetSkyColour());
		uboObject.fogColour = Colour(*Skyboxes::Get()->GetFog()->m_colour);
		uboObject.fogLimits = m_gameObject->GetTransform()->m_scaling->m_y * Vector2(Skyboxes::Get()->GetFog()->m_lowerLimit,
			Skyboxes::Get()->GetFog()->m_upperLimit);
		uboObject.blendFactor = m_blend;
		m_uniformObject->Update(&uboObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{
			uniformScene.GetWriteDescriptor(0, descriptorSet), m_uniformObject->GetWriteDescriptor(1, descriptorSet),
			m_cubemap->GetWriteDescriptor(2, descriptorSet)
		};
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		// Draws the object.
		VkDescriptorSet descriptors[1] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		mesh->GetModel()->CmdRender(commandBuffer);
	}
}
