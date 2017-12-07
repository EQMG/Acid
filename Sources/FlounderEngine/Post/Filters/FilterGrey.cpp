#include "FilterGrey.hpp"

#include "../../Devices/Display.hpp"
#include "../../Renderer/Renderer.hpp"

namespace Flounder
{
	const DescriptorType FilterGrey::typeSamplerColour = Texture::CreateDescriptor(0, VK_SHADER_STAGE_FRAGMENT_BIT);

	FilterGrey::FilterGrey(const int &subpass) :
		IPostFilter("Resources/Shaders/Filters/Grey.frag.spv", subpass, { typeSamplerColour })
	{
	}

	FilterGrey::~FilterGrey()
	{
	}

	void FilterGrey::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
		const auto descriptorSet = m_pipeline->GetDescriptorSet();
		const std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>
		{
			Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(0, descriptorSet)
		};
		IPostFilter::CmdRender(commandBuffer, descriptorWrites);
	}
}
