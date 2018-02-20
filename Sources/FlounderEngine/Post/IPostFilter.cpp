#include "IPostFilter.hpp"

#include "../Devices/Display.hpp"
#include "../Models/Shapes/Rectangle.hpp"

namespace Flounder
{
	const PipelineCreateInfo PIPELINE_CREATE_INFO =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(1), // vertexAttributeDescriptions

			{}, // descriptors

			{"Resources/Shaders/Filters/Default.vert.spv", "Resources/Shaders/Filters/Default.frag.spv"} // shaderStages
		};

	IPostFilter::IPostFilter(const std::string &fragmentShader, const int &subpass, const std::vector<DescriptorType> &descriptors) :
		m_pipeline(nullptr),
		m_model(Rectangle::Resource(-1.0f, 1.0f))
	{
		PipelineCreateInfo pipelineCreateInfo = PipelineCreateInfo(PIPELINE_CREATE_INFO);
		pipelineCreateInfo.shaderStages[1] = fragmentShader; // fragment
		pipelineCreateInfo.descriptors = descriptors; // descriptors
		m_pipeline = new Pipeline(subpass, pipelineCreateInfo);
	}

	IPostFilter::~IPostFilter()
	{
		delete m_model;
		delete m_pipeline;
	}

	void IPostFilter::RenderFilter(const VkCommandBuffer &commandBuffer)
	{
	}

	void IPostFilter::CmdRender(const VkCommandBuffer &commandBuffer, const std::vector<VkWriteDescriptorSet> &descriptorWrites)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		m_pipeline->BindPipeline(commandBuffer);

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = {descriptorSet};
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(commandBuffer);
	}
}
