#include "FilterGrey.hpp"

namespace acid
{
	FilterGrey::FilterGrey(const Pipeline::Stage &pipelineStage) :
		PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Grey.frag"}, {})
	{
	}

	void FilterGrey::Render(const CommandBuffer &commandBuffer)
	{
		// Updates descriptors.
		//	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
		//	m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Draws the object.
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		m_model->CmdRender(commandBuffer);
	}
}
