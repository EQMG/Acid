#include "FilterDefault.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			Texture::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // samplerColour
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // writeColour
		};

	FilterDefault::FilterDefault(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Default.frag.spv", subpass, DESCRIPTORS)
	{
	}

	FilterDefault::~FilterDefault()
	{
	}

	void FilterDefault::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
			{
				Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(0, descriptorSet),
				Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet)
			};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
