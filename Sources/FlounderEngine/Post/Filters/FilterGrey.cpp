#include "FilterGrey.hpp"

#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const std::vector<DescriptorType> DESCRIPTORS =
		{
			Texture::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT), // writeColour
			Texture::CreateDescriptor(1, VK_SHADER_STAGE_FRAGMENT_BIT) // samplerColour
		};

	FilterGrey::FilterGrey(const GraphicsStage &graphicsStage) :
		IPostFilter("Resources/Shaders/Filters/Grey.frag.spv", graphicsStage, DESCRIPTORS)
	{
	}

	FilterGrey::~FilterGrey()
	{
	}

	void FilterGrey::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
		auto descriptorSet = *m_pipeline->GetDescriptorSet();

		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			m_pipeline->GetTexture(2)->GetWriteDescriptor(0, descriptorSet),
			m_pipeline->GetTexture(2)->GetWriteDescriptor(1, descriptorSet)
		};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
