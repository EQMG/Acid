#include "FilterDarken.hpp"

namespace acid
{
FilterDarken::FilterDarken(const Pipeline::Stage &pipelineStage, float factor) :
	PostFilter(pipelineStage, { "Shaders/Post/Default.vert", "Shaders/Post/Darken.frag" }),
	m_factor(factor)
{
}

void FilterDarken::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	m_pushScene.Push("factor", m_factor);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline))
	{
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
