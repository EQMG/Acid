#include "FilterFXAA.hpp"

namespace acid
{
	FilterFXAA::FilterFXAA(const Pipeline::Stage &pipelineStage, const float &spanMax) :
		PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/FXAA.frag"}, {}),
		m_spanMax(spanMax)
	{
	}

	void FilterFXAA::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_pushScene.Push("spanMax", m_spanMax);

		// Updates descriptors.
		m_descriptorSet.Push("PushScene", m_pushScene);
	//	m_descriptorSet.Push("writeColour", GetAttachment("writeColour", "resolved"));
	//	m_descriptorSet.Push("samplerColour", GetAttachment("samplerColour", "resolved"));
		PushConditional("writeColour", "samplerColour", "resolved", "diffuse");
		bool updateSuccess = m_descriptorSet.Update(m_pipeline);

		if (!updateSuccess)
		{
			return;
		}

		// Binds the pipeline.
		m_pushScene.BindPush(commandBuffer, m_pipeline);
		m_pipeline.BindPipeline(commandBuffer);

		// Draws the object.
		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		vkCmdDraw(commandBuffer.GetCommandBuffer(), 3, 1, 0, 0);
	}
}
