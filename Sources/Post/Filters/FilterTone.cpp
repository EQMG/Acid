#include "FilterTone.hpp"

namespace acid
{
FilterTone::FilterTone(const Pipeline::Stage &pipelineStage) :
	PostFilter(pipelineStage, { "Shaders/Post/Default.vert", "Shaders/Post/Tone.frag" }, {})
{
}

void FilterTone::Render(const CommandBuffer &commandBuffer)
{
	// Updates descriptors.
	//m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
	bool updateSuccess = m_descriptorSet.Update(m_pipeline);

	if (!updateSuccess)
	{
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
