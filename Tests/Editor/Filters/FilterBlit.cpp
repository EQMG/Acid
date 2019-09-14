#include "FilterBlit.hpp"

namespace test
{
FilterBlit::FilterBlit(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Blit.frag"})
{
}

void FilterBlit::Render(const CommandBuffer &commandBuffer)
{
	// Updates descriptors.
	m_descriptorSet.Push("samplerColour", Graphics::Get()->GetAttachment("swapchain"));

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
