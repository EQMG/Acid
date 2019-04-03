#include "FilterTiltshift.hpp"

namespace acid
{
FilterTiltshift::FilterTiltshift(const Pipeline::Stage &pipelineStage, const float &blurAmount, const float &centre, const float &stepSize, const float &steps) :
	PostFilter(pipelineStage, { "Shaders/Post/Default.vert", "Shaders/Post/Tiltshift.frag" }, {}),
	m_blurAmount(blurAmount),
	m_centre(centre),
	m_stepSize(stepSize),
	m_steps(steps)
{
}

void FilterTiltshift::Render(const CommandBuffer &commandBuffer)
{
	// Updates uniforms.
	m_pushScene.Push("blurAmount", m_blurAmount);
	m_pushScene.Push("centre", m_centre);
	m_pushScene.Push("stepSize", m_stepSize);
	m_pushScene.Push("steps", m_steps);

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
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
