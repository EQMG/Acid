#include "FilterGrey.hpp"

#include "../../devices/Display.hpp"
#include "../../renderer/Renderer.hpp"

namespace Flounder
{
	FilterGrey::FilterGrey(const int &subpass) :
		IPostFilter("filterGrey", "res/shaders/filters/grey.frag.spv", subpass)
	{
	}

	FilterGrey::~FilterGrey()
	{
	}

	void FilterGrey::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		std::vector<VkWriteDescriptorSet> descriptorWrites = std::vector<VkWriteDescriptorSet>{ Renderer::Get()->GetSwapchain()->GetColourImage()->GetWriteDescriptor(1, descriptorSet) };
		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = { m_pipeline->GetDescriptorSet() };
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(*commandBuffer);
	}
}
