#include "IPostFilter.hpp"

#include "../devices/Display.hpp"
#include "../renderer/buffers/UniformBuffer.hpp"
#include "../textures/Texture.hpp"

namespace Flounder
{
	const std::vector<Vertex> VERTICES =
	{
		Vertex(Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)),
		Vertex(Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		Vertex(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f))
	};
	const std::vector<uint32_t> INDICES =
	{
		0, 3, 2, 2, 1, 0
	};

	const PipelineCreateInfo IPostFilter::pipelineCreateInfo =
	{
		PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
		VK_POLYGON_MODE_FILL, // polygonMode
		VK_CULL_MODE_BACK_BIT, // cullModeFlags

		Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
		Vertex::GetAttributeDescriptions(), // vertexAttributeDescriptions

		{}, // descriptors

		{ "res/shaders/filters/default.vert.spv", "res/shaders/filters/default.frag.spv" } // shaderStages
	};

	IPostFilter::IPostFilter(const std::string &filterName, const std::string &fragmentShader, const int &subpass, const std::vector<DescriptorType> &descriptors) :
		m_pipeline(nullptr),
		m_model(new Model(VERTICES, INDICES))
	{
		PipelineCreateInfo pipelineCreateInfo = PipelineCreateInfo(this->pipelineCreateInfo);
		pipelineCreateInfo.shaderStages[1] = fragmentShader; // fragment
		pipelineCreateInfo.descriptors = descriptors; // descriptors
		m_pipeline = new Pipeline(filterName, pipelineCreateInfo, subpass);
	}

	IPostFilter::~IPostFilter()
	{
		delete m_model;
		delete m_pipeline;
	}

	void IPostFilter::RenderFilter(const VkCommandBuffer *commandBuffer)
	{
	}

	void IPostFilter::CmdRender(const VkCommandBuffer *commandBuffer, const std::vector<VkWriteDescriptorSet> &descriptorWrites)
	{
		const auto logicalDevice = Display::Get()->GetLogicalDevice();
		const auto descriptorSet = m_pipeline->GetDescriptorSet();

		vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipeline());

		vkUpdateDescriptorSets(logicalDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		VkDescriptorSet descriptors[1] = { descriptorSet };
		vkCmdBindDescriptorSets(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetPipelineLayout(), 0, 1, descriptors, 0, nullptr);

		m_model->CmdRender(*commandBuffer);
	}
}
