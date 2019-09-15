#include "FilterPixel.hpp"

namespace acid {
FilterPixel::FilterPixel(const Pipeline::Stage &pipelineStage, float pixelSize) :
	PostFilter(pipelineStage, {"Shaders/Post/Default.vert", "Shaders/Post/Pixel.frag"}),
	m_pixelSize(pixelSize) {
}

void FilterPixel::Render(const CommandBuffer &commandBuffer) {
	// Updates uniforms.
	m_pushScene.Push("pixelSize", m_pixelSize);

	// Updates descriptors.
	m_descriptorSet.Push("PushScene", m_pushScene);
	PushConditional("writeColour", "samplerColour", "resolved", "diffuse");

	if (!m_descriptorSet.Update(m_pipeline)) {
		return;
	}

	// Draws the object.
	m_pipeline.BindPipeline(commandBuffer);

	m_descriptorSet.BindDescriptor(commandBuffer, m_pipeline);
	m_pushScene.BindPush(commandBuffer, m_pipeline);
	vkCmdDraw(commandBuffer, 3, 1, 0, 0);
}
}
