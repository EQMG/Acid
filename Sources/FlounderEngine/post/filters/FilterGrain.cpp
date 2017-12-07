#include "FilterGrain.hpp"

#include "../../textures/Texture.hpp"
#include "../../renderer/Renderer.hpp"

namespace Flounder
{
	const DescriptorType FilterGrain::typeUboScene = UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);
	const DescriptorType FilterGrain::typeSamplerColour = Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT);

	FilterGrain::FilterGrain(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Grain.frag.spv", subpass, { typeUboScene, typeSamplerColour }),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_strength(2.3f)
	{
	}

	FilterGrain::~FilterGrain()
	{
		delete m_uniformScene;
	}

	void FilterGrain::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.strength = m_strength;
		m_uniformScene->Update(&uboScene);

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			m_uniformScene->GetWriteDescriptor(0, descriptorSet),
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet)
		};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
