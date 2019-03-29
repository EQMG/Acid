#include "FilterVignette.hpp"

namespace acid
{
FilterVignette::FilterVignette(const Pipeline::Stage &pipelineStage, const float &innerRadius, const float &outerRadius, const float &opacity) :
	PostFilter(pipelineStage, { "Shaders/Post/Default.vert", "Shaders/Post/Vignette.frag" }, {}),
	m_innerRadius(innerRadius),
	m_outerRadius(outerRadius),
	m_opacity(opacity)
{
}

void FilterVignette::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	m_pushScene.Push("innerRadius", m_innerRadius);
	m_pushScene.Push("outerRadius", m_outerRadius);
	m_pushScene.Push("opacity", m_opacity);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	//m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
	bool updateSuccess = m_descriptorSet.Update(m_pipeline);

	if (!updateSuccess)
	{
		return;
	}

	// Draws the object.
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer.GetCommandBuffer(), 3, 1, 0, 0);
}
}
