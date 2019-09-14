#include "FilterGrey.hpp"

namespace acid
{
FilterGrey::FilterGrey(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Grey.frag"})
{
}

void FilterGrey::Render(const CommandBuffer &commandBuffer)
{
	// Updates descriptors.
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline))
	{
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
