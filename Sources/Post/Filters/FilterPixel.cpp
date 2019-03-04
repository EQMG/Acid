#include "FilterPixel.hpp"

namespace acid
{
	FilterPixel::FilterPixel(const Pipeline::Stage &pipelineStage, const float &pixelSize) :
		PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Pixel.frag"}, {}),
		m_pixelSize(pixelSize)
	{
	}

	void FilterPixel::Render(const CommandBuffer &commandBuffer)
	{
		// Updates uniforms.
		m_pushScene.Push("pixelSize", m_pixelSize);

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

		// Draws the object.
		m_pushScene.BindPush(commandBuffer, m_pipeline);
		m_pipeline.BindPipeline(commandBuffer);

		m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
		vkCmdDraw(commandBuffer.GetCommandBuffer(), 3, 1, 0, 0);
	}
}
