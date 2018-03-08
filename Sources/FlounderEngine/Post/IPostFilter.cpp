#include "IPostFilter.hpp"

#include "../Devices/Display.hpp"
#include "../Models/Shapes/ShapeRectangle.hpp"

namespace Flounder
{
	const PipelineCreate PIPELINE_CREATE =
		{
			PIPELINE_POLYGON_NO_DEPTH, // pipelineModeFlags
			VK_POLYGON_MODE_FILL, // polygonMode
			VK_CULL_MODE_BACK_BIT, // cullModeFlags

			Vertex::GetBindingDescriptions(), // vertexBindingDescriptions
			Vertex::GetAttributeDescriptions(1), // vertexAttributeDescriptions

			{}, // descriptors

			{"Resources/Shaders/Filters/Default.vert.spv", "Resources/Shaders/Filters/Default.frag.spv"} // shaderStages
		};

	IPostFilter::IPostFilter(const std::string &fragmentShader, const GraphicsStage &graphicsStage, const std::vector<DescriptorType> &descriptors) :
		m_pipeline(nullptr),
		m_descriptorSet(nullptr),
		m_model(ShapeRectangle::Resource(-1.0f, 1.0f))
	{
		PipelineCreate pipelineCreateInfo = PipelineCreate(PIPELINE_CREATE);
		pipelineCreateInfo.shaderStages[1] = fragmentShader; // fragment
		pipelineCreateInfo.descriptors = descriptors; // descriptors
		m_pipeline = new Pipeline(graphicsStage, pipelineCreateInfo);
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
		if (m_descriptorSet == nullptr)
		{
			m_descriptorSet = new DescriptorSet(*m_pipeline);
			m_descriptorSet->Update(descriptorWrites);
		}

		m_pipeline->BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet->BindDescriptor(commandBuffer, *m_pipeline);
		m_model->CmdRender(commandBuffer);
	}
}
