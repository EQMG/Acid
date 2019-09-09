#include "FilterWobble.hpp"

namespace acid
{
FilterWobble::FilterWobble(const Pipeline::Stage &pipelineStage, float wobbleSpeed) :
	PostFilter{pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Wobble.frag"}},
	m_wobbleSpeed{wobbleSpeed},
	m_wobbleAmount{0.0f}
{
}

void FilterWobble::Render(const CommandBuffer &commandBuffer)
{
	m_wobbleAmount += m_wobbleSpeed * Engine::Get()->GetDeltaRender().AsSeconds();

	// Updates uniforms.
	m_pushScene.Push("moveIt", m_wobbleAmount);

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
