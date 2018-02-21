#include "FilterDarken.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			UniformBuffer::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // uboScene
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(2, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterDarken::FilterDarken(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Darken.frag.spv", subpass, DESCRIPTORS),
		m_uniformScene(new UniformBuffer(sizeof(UboScene))),
		m_factor(0.5f)
	{
	}

	FilterDarken::~FilterDarken()
	{
		delete m_uniformScene;
	}

	void FilterDarken::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		UboScene uboScene = {};
		uboScene.factor = m_factor;
		m_uniformScene->Update(&uboScene);

		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
			{
				m_uniformScene->GetWriteDescriptor(0, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(2, descriptorSet)
			};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
