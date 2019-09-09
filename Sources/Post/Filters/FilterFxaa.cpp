#include "FilterFxaa.hpp"

namespace acid
{
FilterFxaa::FilterFxaa(const Pipeline::Stage &pipelineStage, float spanMax) :
	PostFilter{pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Fxaa.frag"}},
	m_spanMax{spanMax}
{
}

void FilterFxaa::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	m_pushScene.Push("spanMax", m_spanMax);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline))
	{
		return;
	}

	// Binds the pipeline.
	m_pipeline.BindPipeline(commandBuffer);

	// Draws the object.
	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
